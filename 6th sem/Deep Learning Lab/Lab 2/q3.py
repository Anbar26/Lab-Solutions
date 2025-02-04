import torch
def sigmoid(a):
    return 1.0 / (1.0 + torch.exp(-1 * a));


def calc_grad(b, x, w, u, v, a):
    dadv = a * (1 - a)
    dvdu = 1
    dudw = x

    dadw = dadv * dvdu * dudw
    return dadw.item()


b = torch.tensor(0.03, requires_grad=True)
x = torch.tensor(1.01, requires_grad=True)
w = torch.tensor(4.052, requires_grad=True)

u = w * x
v = u + b
a = sigmoid(v)

a.backward()
print("torch gradient: ", w.grad.item())
print("manual gradient: ", calc_grad(b, x, w, u, v, a))