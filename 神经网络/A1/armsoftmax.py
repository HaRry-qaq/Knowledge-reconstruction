#! /usr/bin/python
# -*- encoding: utf-8 -*-
# Adapted from https://github.com/CoinCheung/pytorch-loss (MIT License)

import torch
import torch.nn as nn
import torch.nn.functional as F
import pdb
try:
    from .utils import accuracy
except:
    from utils import accuracy

class LossFunction(nn.Module):
    def __init__(self, embedding_dim = 10, num_classes = 10, margin=0.2, scale=30, **kwargs):
        super(LossFunction, self).__init__()

        self.m = margin
        self.s = scale
        self.in_feats = embedding_dim
        self.W = torch.nn.Parameter(torch.randn(embedding_dim, num_classes), requires_grad=True)
        self.ce = nn.CrossEntropyLoss()
        nn.init.xavier_normal_(self.W, gain=1)

        print('Initialised ANRM-Softmax m=%.3f s=%.3f'%(self.m, self.s))

    def forward(self, x, label=None):
        assert len(x.shape) == 3
        label = label.repeat_interleave(x.shape[1])
        x = x.reshape(-1, self.in_feats)
        assert x.size()[0] == label.size()[0]
        assert x.size()[1] == self.in_feats
        # print(x)
        # print(label)

        x_norm = torch.norm(x, p=2, dim=1, keepdim=True).clamp(min=1e-12)
        x_norm = torch.div(x, x_norm)
        w_norm = torch.norm(self.W, p=2, dim=0, keepdim=True).clamp(min=1e-12)
        w_norm = torch.div(self.W, w_norm)

        device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
        w_norm = w_norm.to(device)
        costh = torch.mm(x_norm, w_norm)
        label_view = label.view(-1, 1)
        if label_view.is_cuda: label_view = label_view.cpu()
        delt_costh = torch.zeros(costh.size()).scatter_(1, label_view, self.m)
        if x.is_cuda: delt_costh = delt_costh.cuda()
        costh_m = costh - delt_costh
        costh_m_s = self.s * costh_m
        # print(costh_m_s)

        if costh_m_s.is_cuda: label_view=label_view.cuda()
        delt_costh_m_s = costh_m_s.gather(1, label_view).repeat(1,costh_m_s.size()[1])

        costh_m_s_reduct = costh_m_s - delt_costh_m_s
        # print(costh_m_s_reduct)

        costh_relu = torch.where(costh_m_s_reduct < 0.0, torch.zeros_like(costh_m_s), costh_m_s)
        # print(costh_relu)
        
        loss    = self.ce(costh_relu, label)
        prec1   = accuracy(costh_relu.detach(), label.detach(), topk=(1,))[0]
        return loss, prec1


if __name__ == '__main__':
    criteria = LossFunction(20,5)
    a = torch.ones(10, 1, 20)
    # print(a)
    lb = torch.randint(0, 5, (10, ), dtype=torch.long)
    # print(lb)
    loss, prec = criteria(a, lb)

