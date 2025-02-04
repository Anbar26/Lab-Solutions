import torch
def calc_grad(x, y):
    dydx = 32*x**3 + 9*x**2 + 14*x + 6
    return dydx.item()

x = torch.tensor(24.5, requires_grad=True)
y = 8*x**4 + 3*x**3 + 7*x**2 + 6*x + 3

y.backward()
print("torch gradient: ", x.grad.item())
print("manual gradient: ", calc_grad(x, y))