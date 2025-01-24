import torch
import matplotlib.pyplot as plt


class RegressionModel:
    def __init__(self):
        # Initialize parameters (w and b) with value 1
        self.w = torch.tensor(1.0, requires_grad=True)
        self.b = torch.tensor(1.0, requires_grad=True)

    def forward(self, x):
        # Linear model: y_pred = wx + b
        return self.w * x + self.b

    def update(self, learning_rate):
        # Update parameters using gradient descent: w -= learning_rate * w.grad
        with torch.no_grad():
            self.w -= learning_rate * self.w.grad
            self.b -= learning_rate * self.b.grad

    def reset_grad(self):
        # Reset gradients to zero for the next iteration
        self.w.grad.zero_()
        self.b.grad.zero_()

    def criterion(self, y, yp):
        # MSE loss: (1/N) * sum((y_pred - y_true) ** 2)
        loss = torch.mean((yp - y) ** 2)
        return loss


# Define the data
x = torch.tensor([5.0, 7.0, 12.0, 16.0, 20.0])
y = torch.tensor([40.0, 120.0, 180.0, 210.0, 240.0])

# Learning rate
learning_rate = torch.tensor(0.001)

# Create the model
model = RegressionModel()

# Track the loss for each epoch
losses = []

# Number of epochs
epochs = 100

# Training loop
for epoch in range(epochs):
    # Forward pass: Compute predicted y
    y_pred = model.forward(x)

    # Compute the loss
    loss = model.criterion(y, y_pred)

    # Backward pass: Compute gradients
    loss.backward()

    # Update parameters
    model.update(learning_rate)

    # Reset gradients for the next iteration
    model.reset_grad()

    # Store the loss
    losses.append(loss.item())

# Plot the loss over epochs
plt.plot(range(epochs), losses)
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.title('Epoch vs Loss')
plt.show()
