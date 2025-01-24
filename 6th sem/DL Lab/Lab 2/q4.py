import torch
def calc_grad(x, a, b, c, d, e):
    dedd = e;
    dddc = 1;
    dcdx = -2 * x;
    dddb = 1;
    dbdx = -2;
    ddda = -1;
    dadx = torch.cos(x)

    dedx = dedd * dddc * dcdx + dedd * dddb * dbdx + dedd * ddda * dadx
    return dedx.item()


x = torch.tensor(0.315, requires_grad=True)

a = torch.sin(x)
b = -2 * x
c = -1 * x ** 2
d = c + b - a
e = torch.exp(d)

e.backward()
print("torch gradient: ", x.grad.item())
print("manual gradient: ", calc_grad(x, a, b, c, d, e))