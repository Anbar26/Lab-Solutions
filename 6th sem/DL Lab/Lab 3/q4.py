import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader
import matplotlib.pyplot as plt


class LinearRegressionDataset(Dataset):
    def __init__(self, x_data, y_data):
        self.x_data = x_data
        self.y_data = y_data

    def __len__(self):
        return len(self.x_data)

    def __getitem__(self, idx):
        return self.x_data[idx], self.y_data[idx]


class RegressionModel(nn.Module):
    def __init__(self):
        super(RegressionModel, self).__init__()
        self.w = nn.Parameter(torch.tensor(1.0))  # Model weight
        self.b = nn.Parameter(torch.tensor(1.0))  # Model bias

    def forward(self, x):
        return self.w * x + self.b


# Define the dataset
x = torch.tensor([5.0, 7.0, 12.0, 16.0, 20.0])
y = torch.tensor([40.0, 120.0, 180.0, 210.0, 240.0])

# Create a custom dataset
dataset = LinearRegressionDataset(x, y)

# DataLoader for batching and shuffling
dataloader = DataLoader(dataset, batch_size=1, shuffle=True)

# Create the model
model = RegressionModel()

# Define the loss function (Mean Squared Error)
criterion = nn.MSELoss()

# Use SGD optimizer
optimizer = optim.SGD(model.parameters(), lr=0.001)

# List to track loss over epochs
losses = []

# Number of epochs
epochs = 100

for epoch in range(epochs):
    epoch_loss = 0
    for data in dataloader:
        x_batch, y_batch = data

        # Zero gradients
        optimizer.zero_grad()

        # Forward pass: compute predictions
        y_pred = model(x_batch)

        # Compute the loss
        loss = criterion(y_pred, y_batch)

        # Backward pass: compute gradients
        loss.backward()

        # Update the parameters
        optimizer.step()

        # Accumulate loss for the epoch
        epoch_loss += loss.item()

    # Average loss for the epoch
    losses.append(epoch_loss / len(dataloader))

# Plotting the loss over epochs
plt.plot(range(epochs), losses)
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.title('Epoch vs Loss')
plt.show()
