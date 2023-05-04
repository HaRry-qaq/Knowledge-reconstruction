#! /usr/bin/python
# -*- encoding: utf-8 -*-
# Adapted from https://github.com/CoinCheung/pytorch-loss (MIT License)

import torch
import torch.nn as nn
import torch.nn.functional as F
try:
    from .utils import accuracy
except:
    from utils import accuracy

class LossFunction(nn.Module):
    # 改变baseline或multi task 只要更改embedding_dim
    def __init__(self, embedding_dim = 10, num_classes = 10, margin=0.2, scale=30, **kwargs):
        super(LossFunction, self).__init__()

        self.m = margin
        self.s = scale
        self.in_feats = embedding_dim
        self.W = torch.nn.Parameter(torch.randn(embedding_dim, num_classes), requires_grad=True)
        self.ce = nn.CrossEntropyLoss()
        nn.init.xavier_normal_(self.W, gain=1)

        print('Initialised AM-Softmax m=%.3f s=%.3f'%(self.m, self.s))

    def forward(self, x, label=None):
        assert len(x.shape) == 3
        label = label.repeat_interleave(x.shape[1])
        x = x.reshape(-1, self.in_feats)
        assert x.size()[0] == label.size()[0]
        assert x.size()[1] == self.in_feats

        x_norm = torch.norm(x, p=2, dim=1, keepdim=True).clamp(min=1e-12)
        x_norm = torch.div(x, x_norm)
        w_norm = torch.norm(self.W, p=2, dim=0, keepdim=True).clamp(min=1e-12)
        w_norm = torch.div(self.W, w_norm)

        # print(x_norm.is_cuda, w_norm.is_cuda)


        device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
        w_norm = w_norm.to(device)

        # print(x_norm.is_cuda, w_norm.is_cuda)

        costh = torch.mm(x_norm, w_norm)
        label_view = label.view(-1, 1)
        if label_view.is_cuda: label_view = label_view.cpu()
        delt_costh = torch.zeros(costh.size()).scatter_(1, label_view, self.m)
        if x.is_cuda: delt_costh = delt_costh.cuda()
        costh_m = costh - delt_costh
        costh_m_s = self.s * costh_m
        loss    = self.ce(costh_m_s, label)
        prec1   = accuracy(costh_m_s.detach(), label.detach(), topk=(1,))[0]
        return loss, prec1

