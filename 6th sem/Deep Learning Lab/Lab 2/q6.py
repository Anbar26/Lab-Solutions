import torch
def calc_grad(x, y, z, a, b, c, d, e, f):
    dfde = 1-torch.tanh(e)**2
    dedd = 1/(d+1); dddc = z
    dcdb = -1*a/b**2
    dbdy = torch.cos(y)
    dfdy = dfde*dedd*dddc*dcdb*dbdy
    return dfdy.item()

x = torch.tensor(2.6, requires_grad=True)
y = torch.tensor(6.5, requires_grad=True)
z = torch.tensor(2.26, requires_grad=True)

a = 2*x
b = torch.sin(y)
c = a/b
d = z*c
e = torch.log(d+1)
f = torch.tanh(e)

f.backward()
print("torch gradient: ", y.grad.item())
print("manual gradient: ", calc_grad(x, y, z, a, b, c, d, e, f))