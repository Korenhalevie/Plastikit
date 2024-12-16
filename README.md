# Plastikit

![Plastikit Banner](https://via.placeholder.com/1200x300?text=Plastikit+Recycling+Project)

## 🌍 Overview

**Plastikit** is an innovative project that won a hackathon and received funding from the Ramat Gan municipality. The goal of the project is to promote sustainability by recycling plastic bottles and converting them into filament for 3D printers. This initiative combines environmental awareness with technology to engage children and the community in recycling efforts.

---

## 🎯 Project Features

- **♻️ Plastic Recycling to 3D Printing**:  
   Plastikit facilitates the process of melting down plastic bottles to create usable filament for 3D printing.

- **📡 IoT Integration with MQTT**:  
   The system uses sensors to collect data about the recycling process.  
   This data is transmitted via the MQTT protocol to a central server.

- **👩‍💻 Interactive Feedback for Users**:  
   Children and users can monitor their recycling contributions through the server.  
   The system provides feedback, showing how much they have recycled and what 3D-printed objects they can create based on their contributions.

---

## 📂 Project Structure

- **`include/`**: Contains header files defining interfaces and functionalities.
- **`lib/`**: Includes shared libraries and dependencies used in the project.
- **`src/`**: Houses the core logic and implementation code.
- **`test/`**: Provides unit tests and validation scripts to ensure code reliability.
- **`.vscode/`**: Configurations for Visual Studio Code, including build tasks and debugging setups.

---

## 🛠️ Key Technologies

- **C and C++**: For efficient and fast implementation of core functionalities.
- **MQTT Protocol**: Ensures reliable and lightweight communication between sensors and the server.
- **PlatformIO**: Used for development and deployment of embedded systems.

---

## 💻 Requirements

- **Development Environment:**
  - PlatformIO IDE (recommended)
  - Compatible C/C++ compilers

- **Hardware:**
  - Sensors for measuring and monitoring recycling activity
  - IoT-enabled devices for MQTT communication

---

## 🚀 Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/Korenhalevie/Plastikit.git