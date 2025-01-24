import torch

# Given data
x = torch.tensor([2.0, 4.0], dtype=torch.float32)
y = torch.tensor([20.0, 40.0], dtype=torch.float32)

# Initialize parameters
w = torch.tensor(1.0, requires_grad=True)
b = torch.tensor(1.0, requires_grad=True)

# Learning rate
learning_rate = 0.001


# Function to compute the loss
def compute_loss(x, y, w, b):
    y_pred = w * x + b
    loss = torch.mean((y_pred - y) ** 2)  # MSE loss
    return loss


# Perform training for two epochs
for epoch in range(2):
    # Forward pass: compute predicted y
    y_pred = w * x + b

    # Compute loss
    loss = compute_loss(x, y, w, b)

    # Backward pass: compute gradients
    loss.backward()

    # Print gradients for w and b
    print(f"Epoch {epoch + 1}:")
    print(f"w.grad: {w.grad.item()}, b.grad: {b.grad.item()}")

    # Update parameters
    with torch.no_grad():
        w -= learning_rate * w.grad
        b -= learning_rate * b.grad

    # Zero the gradients for the next step
    w.grad.zero_()
    b.grad.zero_()

    # Print updated values of w and b
    print(f"Updated w: {w.item()}, Updated b: {b.item()}")
    print("-" * 40)
