import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets, transforms
from sklearn.metrics import confusion_matrix
import seaborn as sns
import matplotlib.pyplot as plt

# 1. Load MNIST dataset
transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.5,), (0.5,))])

trainset = datasets.MNIST(root='./data', train=True, download=True, transform=transform)
testset = datasets.MNIST(root='./data', train=False, download=True, transform=transform)

trainloader = torch.utils.data.DataLoader(trainset, batch_size=64, shuffle=True)
testloader = torch.utils.data.DataLoader(testset, batch_size=64, shuffle=False)

# 2. Define the Feed Forward Neural Network (FFNN) with two hidden layers
class FFNN(nn.Module):
    def __init__(self):
        super(FFNN, self).__init__()
        self.fc1 = nn.Linear(28 * 28, 128)  # First hidden layer (784 -> 128 neurons)
        self.fc2 = nn.Linear(128, 128)      # Second hidden layer (128 -> 128 neurons)
        self.fc3 = nn.Linear(128, 10)       # Output layer (128 -> 10 classes)
        self.relu = nn.ReLU()
        self.softmax = nn.Softmax(dim=1)

    def forward(self, x):
        x = x.view(-1, 28 * 28)  # Flatten the input images (batch_size, 28*28)
        x = self.relu(self.fc1(x))  # Apply ReLU activation on the first layer
        x = self.relu(self.fc2(x))  # Apply ReLU activation on the second layer
        x = self.fc3(x)  # Output layer (logits)
        return x

# 3. Initialize the model, loss function, and optimizer
model = FFNN()
criterion = nn.CrossEntropyLoss()  # For multi-class classification
optimizer = optim.Adam(model.parameters(), lr=0.001)

# 4. Training the model
epochs = 5
for epoch in range(epochs):
    model.train()
    running_loss = 0.0
    for inputs, labels in trainloader:
        optimizer.zero_grad()  # Zero out gradients
        outputs = model(inputs)  # Forward pass
        loss = criterion(outputs, labels)  # Calculate the loss
        loss.backward()  # Backpropagate the error
        optimizer.step()  # Update the weights

        running_loss += loss.item()

    avg_loss = running_loss / len(trainloader)
    print(f"Epoch {epoch+1}/{epochs}, Loss: {avg_loss:.4f}")

# 5. Evaluate the model
model.eval()
correct = 0
total = 0
y_true = []
y_pred = []

with torch.no_grad():  # No gradients are calculated during evaluation
    for inputs, labels in testloader:
        outputs = model(inputs)
        _, predicted = torch.max(outputs.data, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item()
        y_true.extend(labels.numpy())
        y_pred.extend(predicted.numpy())

test_accuracy = 100 * correct / total
print(f"Test accuracy: {test_accuracy:.2f}%")

# 6. Compute the confusion matrix
cm = confusion_matrix(y_true, y_pred)

# Plot the confusion matrix
plt.figure(figsize=(10,7))
sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', xticklabels=list(range(10)), yticklabels=list(range(10)))
plt.title("Confusion Matrix")
plt.xlabel('Predicted Label')
plt.ylabel('True Label')
plt.show()

# 7. Verify the number of learnable parameters in the model
num_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
print(f"Total number of learnable parameters: {num_params}")
