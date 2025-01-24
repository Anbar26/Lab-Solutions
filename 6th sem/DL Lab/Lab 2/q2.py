import torch
def relu(a):
    return max(0, a)


def calc_grad(b, x, w, u, v, a):
    dadv = 1 if a > 0 else 0
    dvdu = 1
    dudw = x

    dadw = dadv * dvdu * dudw
    return dadw.item()


b = torch.tensor(5.0, requires_grad=True)
x = torch.tensor(6.0, requires_grad=True)
w = torch.tensor(7.0, requires_grad=True)

u = w * x
v = u + b
a = relu(v)

a.backward()
print("torch gradient: ", w.grad.item())
print("manual gradient: ", calc_grad(b, x, w, u, v, a))