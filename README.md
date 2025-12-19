# AAU-Park (Parking Lot Manager for AAU) 

## P1 - A Product that Solves a Problem
## Parking Lots 

Our first semester project where the task is to develop a product that solves a problem. It needs to be written in C and have some sort of User Interface (UI).
We chose to make a Graphical UI (GUI), by using Raylib and Raygui. 

---

## Overview

The product is build using CMake. Therefore its possible to break the code into files and directories. 
The directories included is: 

### Product Files
- assets
  - Includes files none compiled files
- src
  - Includes all the compiled files in the product.

### Testing Files
- cmake
  - Includes files from [mtest](https://github.com/MortenSchou/mtest]) (Our IMPR lecturer, Morten Konggaard Schou)
- test
  - Includes our testing files

---

## User Manual

### Getting Started

#### Installation
1. **Clone the repository:**
   ```bash
   git clone https://github.com/FedDreng/p1.git
   cd p1
   ```

2. **Build the project:**
   ```bash
   mkdir build
   cd build
   cmake .. -G Ninja 
   ninja
   ```

3. **Run the application:**
   ```bash
   ./p1
   ```
   ```

##### Otherwise you can use Clion to do the same
1. **Clone the repository**
2. **Build**
3. **Run Program**

#### System Requirements
- **Build Tools:** CMake 3.x or higher, C compiler (GCC/MSVC)
- **Dependencies:** 
  - Raylib graphics library   (Gets downloaded using CMake)
  - RayGUI for user interface (Gets downloaded using CMake)
  - WinHTTP or cURL depending on OS

#### Initial Setup
1. **Configure parking lot data:** Ensure the `../assets/parkingLots` file contains your parking lot grid configuration
2. **User profiles:** The system will create `../assets/userPreferences.txt` to store user profiles
3. **Occupied spots:** The system tracks occupied spots in `../assets/occipied.txt`

---

### How to Use
#### 1. Go Through first setup.
- Let the code run,
- Setup car using input fields, Size, Input 
- (License plate affects the input, because license plate insertion is not working in this version)
#### 2. **Create or Select User Profile**
When you first launch AAU-Park, you can create a new user profile or select an existing one: 
- Navigate to **User Profiles** from the main menu
- Click **Create New Profile** to set up a new account
- Enter your username and license plate number
- Configure your preferences (handicap status, EV status, location preferences)

#### 3. **Enter License Plate for Automatic Vehicle Detection**
The system can automatically detect your vehicle type: 
- Enter your Danish license plate number
- The system connects to the Motor API to retrieve: 
  - Vehicle size (Hatchback, Sedan, SUV)
  - Fuel type (Electric or conventional)
- This information helps assign the most suitable parking spot

#### 4. **Manual Vehicle Configuration**
If automatic detection is unavailable, manually specify:
- **Car Size:** Small, Medium, or Large
- **Car Type:** Normal, Electric, Handicap, or Both (Electric + Handicap)

#### 5. **Automatic Parking Assignment**
Once your vehicle details are entered:
- Click **Assign** to let the system find the optimal parking spot
- The system uses an intelligent scoring algorithm that considers:
  - Vehicle size compatibility
  - Electric charging needs
  - Handicap accessibility
  - Distance to university/exit
  - Isolation preferences
- Your assigned spot will blink on the parking grid

#### 6. **Manual Spot Selection**
Prefer to choose your own spot? 
- Navigate to **Choose Spot** mode
- Click on any available parking space on the grid
- The system verifies your vehicle fits before confirming

#### 7. **Check Parking Lot Busyness**
Plan your arrival time:
- Select **Choose Busyness** from the navigation menu
- Choose a day of the week (Monday - Sunday)
- Select a time interval (6 intervals covering 24 hours)
- View predicted busyness levels: 
  - **Level 1:** Not very busy
  - **Level 2:** Moderately busy
  - **Level 3:** Very busy

---

### Key Features

#### **Parking Assignment Algorithm**
The system scores each parking spot based on multiple factors: 

- **Electric Vehicle Priority** (+700 points): EV cars are matched with charging spots
- **Handicap Accessibility** (+600 points): Handicap-designated vehicles get priority for handicap spots
- **Size Optimization** (+300/250/200 points): Cars are matched to appropriately sized spots
  - Perfect fit:  +300 points
  - Acceptable fit: +250 points
  - Can fit but not ideal: +200 points
- **Isolation Preference** (0-90 points): Some users prefer spots with empty adjacent spaces
- **Distance Optimization**:  Spots closer to university/exit receive higher scores

**Impossible Assignments** (-1,000,000 points) include:
- Occupied spots
- Road/obstacle areas
- Spots too small for the vehicle
- Handicap spots for non-handicap vehicles

#### **Parking Grid Visualization**
- Color-coded parking spots: 
  - **Green:** Available parking
  - **Red:** Occupied
  - **Blue:** EV charging spots
  - **Yellow:** Handicap accessible
  - **Gray:** Roads/obstacles
- Blinking indicator shows your assigned spot
- Visual feedback when no suitable spot is available

#### **User Profile Management**
Save and customize your preferences:
- **Username:** Personal identifier
- **License Plate:** For automatic vehicle detection
- **Handicap Status:** Access to handicap-designated spots
- **EV Status:** Priority for charging stations
- **Location Preference:** Prefer spots close to university or exit (0-9 scale)
- **Isolation Preference:** Prefer spots with empty adjacent spaces (0-9 scale)

#### **Busyness Prediction**
Historical data helps you plan optimal parking times:
- 7-day weekly schedule
- 6 time intervals per day (4-hour blocks)
- Visual representation of expected parking availability

---

### Troubleshooting

#### **Issue: License plate API returns no data**
**Solution:** 
- Verify you are using a valid Danish license plate format
- Check internet connection for API access
- Manually enter vehicle information instead

#### **Issue: "No suitable parking spot available" message**
**Solution:** 
- Try manual spot selection via **Choose Spot** mode
- Adjust your preferences to be more flexible
- Check back later when spots may become available

#### **Issue: Application doesn't start**
**Solution:** 
- Ensure all asset files exist in `../assets/` directory: 
  - `parkingLots` (parking grid configuration)
  - `userPreferences.txt` (user profiles)
  - `occipied.txt` (occupied spot tracker)
- Verify Raylib and RayGUI are properly installed
- Check CMake build completed without errors

#### **Issue: Assigned spot doesn't fit my vehicle**
**Solution:** 
- The `calculateLotScore()` function uses the `canFit()` checker
- If this occurs, report as a bug to the development team
- Use manual selection as a workaround

#### **Issue: User profile not saving**
**Solution:** 
- Check file write permissions for `../assets/userPreferences.txt`
- Ensure username and license plate fields are not empty
- Verify no special characters that might corrupt the file format

#### Issue: cURL error
**Solution:**
- Make sure cURL is installed
- Try build again and download cURL lib manual

---

### FAQ

**Q: How does the automatic parking assignment work?**  
A: The system evaluates every available parking spot using a scoring algorithm. It considers your vehicle size, type (EV/handicap), and personal preferences to find the optimal match. The spot with the highest score is automatically assigned.

**Q: Can I save multiple vehicle profiles?**  
A: Yes! The user preference system supports multiple profiles. Each profile stores a unique username, license plate, and preferences. Switch between profiles anytime via the **Change Current Profile** option.

**Q: What happens if I park in a manually selected spot that's too small?**  
A: The system performs a `canFit()` validation before confirming manual selections. If your vehicle doesn't fit, you will see an error message and the spot will not be marked as occupied.

**Q: How accurate is the busyness prediction?**  
A: The current version uses a predefined schedule based on typical university patterns. Future versions may incorporate real-time data and/or machine learning for improved accuracy.

**Q: Can I release my parking spot when I leave?**  
A: Currently, you need to manually edit the `occipied.txt` file to remove your entry. A future update will add a "Release Spot" button in the UI or automatically do it using a physical function (Camera, ultrasonic vehicle detection or Wired Network-based SPS).

**Q: Does the system work for other universities or parking lots?**  
A: While designed for AAU, the system can be adapted by replacing the parking grid configuration in `../assets/parkingLots` with your other layouts.

**Q: What if there is an electric car but no EV spots available?**  
A: The algorithm will assign a regular spot if no EV spots are available, though with a lower score. EV spots are reserved for electric vehicles (non-EV cars get heavily penalized for taking them), but if no spot is available for the regular car it will get an EV spot, but NEVER an handicapped spot.

---

### Support

- **Technical Support:** Contact the development team members (see below)
- **Report Issues:** [GitHub Issues](https://github.com/FedDreng/p1/issues)
- **Feature Requests:** Open a GitHub issue with the "enhancement" label
- **Source Code:** [https://github.com/FedDreng/p1](https://github.com/FedDreng/p1)

---

## Goal

The goal of this project is to design and develop a solution that reduces the time and frustration spent searching for parking. The product will leverage modern technologies to provide real-time insights, smart navigation, or optimized parking management.

---

## Version Control

This repository is used to manage and track the development of our product using Git. It includes code, and other resources related to the project.

---

## Team

[MikkelDis](https://github.com/MikkelDis)
[christianol1234](https://github.com/christianol1234)
[bila0418](https://github.com/bila0418)
[LukasLLaebel](https://github.com/LukasLLaebel/)
[BurnsAau](https://github.com/BurnsAau)
[FredSullz](https://github.com/FredSullz)
[FedDreng](https://github.com/FedDreng)

