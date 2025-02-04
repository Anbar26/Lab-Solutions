import torch
import torch.nn as nn
import torch.optim as optim
import matplotlib.pyplot as plt


# Define the logistic regression model
class LogisticRegressionModel(nn.Module):
    def __init__(self):
        super(LogisticRegressionModel, self).__init__()
        # Linear layer with 1 input feature and 1 output (since we're doing binary classification)
        self.linear = nn.Linear(1, 1)  # 1 input feature, 1 output

    def forward(self, x):
        # Apply the sigmoid function to the output of the linear layer to get a value between 0 and 1
        return torch.sigmoid(self.linear(x))


# Input data (x) and target labels (y)
x_data = torch.tensor([[1.0], [5.0], [10.0], [10.0], [25.0], [50.0], [70.0], [75.0], [100.0]])  # Features
y_data = torch.tensor([[0.0], [0.0], [0.0], [0.0], [0.0], [1.0], [1.0], [1.0], [1.0]])  # Labels (0 or 1)

# Initialize the model
model = LogisticRegressionModel()

# Define the loss function (Binary Cross-Entropy Loss for binary classification)
criterion = nn.BCELoss()

# Define the optimizer (Stochastic Gradient Descent)
optimizer = optim.SGD(model.parameters(), lr=0.001)

# Number of epochs for training
epochs = 1000
losses = []  # List to store loss values for plotting

# Training the model
for epoch in range(epochs):
    # Forward pass: Get the model's prediction
    y_pred = model(x_data)

    # Calculate the loss using the predicted values and true labels
    loss = criterion(y_pred, y_data)

    # Backward pass: Compute gradients for each parameter
    loss.backward()

    # Update the model's parameters using the optimizer
    optimizer.step()

    # Reset gradients to zero for the next step
    optimizer.zero_grad()

    # Store the loss for plotting
    losses.append(loss.item())

    # Print the loss every 100 epochs
    if (epoch + 1) % 100 == 0:
        print(f"Epoch {epoch + 1}/{epochs}, Loss: {loss.item()}")

# Plot the loss curve to visualize the training process
plt.plot(range(epochs), losses)
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.title('Epoch vs Loss')
plt.show()

# Test the trained model with a new input value
new_data = torch.tensor([[30.0]])  # Example input for prediction
prediction = model(new_data).item()  # Get the predicted probability
print(f"Prediction for x=30: {prediction}")
