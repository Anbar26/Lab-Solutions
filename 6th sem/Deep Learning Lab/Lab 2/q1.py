import torch
def calc_grad(a, b, x, y, z):
    dzdx = 2; dzdy=3; dxda=2; dxdb=3; dyda=10*a; dydb=9*b*b
    dzda = dzdx*dxda + dzdy*dyda
    return dzda.item()

a = torch.tensor(5.0, requires_grad=True)
b = torch.tensor(6.0, requires_grad=True)

x = 2*a + 3*b
y = 5*a*a + 3*b*b*b
z = 2*x + 3*y

z.backward()
print("torch gradient: ", a.grad.item())
print("manual gradient: ", calc_grad(a, b, x, y, z))
