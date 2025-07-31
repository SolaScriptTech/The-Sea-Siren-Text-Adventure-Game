// ================================================
// The Sea Siren Text Adventure Game
// Matt Johns - CISP 360
// ================================================
// This is a text-based adventure game where the player explores the wreck of the Sea Siren,
// a sunken luxury cruise liner, encountering various rooms, items, and dangers.
// PQ: Passenger Quarters, HW: Hallway, SW: Stairwell, GB: Grand Ballroom,
// OQ: Officer's Quarters, OD: Observation Deck, BH: Ballast Hall,
// ER: Engine Room, DH: Dining Hall, DR: Dining Room, GB: Gangsters Boat
// ================================================

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

// Don't forget to add whitespace

// Greeting function
void ProgramGreeting() {
    cout << "\n===========================================" << endl;
    cout << "    Exploring a Long Forgotten Watery Tomb" << endl;
    cout << "===========================================\n" << endl;
    cout << "You descend into the wreckage of the Sea Siren,\n"
         << "a once-luxurious cruise liner that mysteriously sunk on its maiden voyage.\n"
         << "The many lost souls of the passengers that once enjoyed the ship's many luxuries\n"
         << "now find themselves trapped in this watery grave site.\n"
         << "Down here, death awaits around every corner and secrets once thought lost forever\n"
         << "now wait to be discovered. Escape... if you can.\n" << endl;
    cout << "You slip through a open porthole in the side of the Sea Siren Luxury Cruise Liner..." << endl;
    cout << "As you enter through the opening the porthole slams shut behind you.\n"
         << "The silence is thick as you realize that there is no going back now.\n" << endl;
    cout << "Created by: Matt Johns\n" << endl;
}

// Inventory and item check
vector<string> inventory;
bool hasItem(const string& item) {
    for (const auto& i : inventory) if (i == item) return true;
    return false;
}

// Room structure
struct Room {
    int id;
    string name;
    string desc;
    string reentryDesc;
    vector<string> items;
    int north = -1, south = -1, east = -1, west = -1, up = -1, down = -1;
    bool visited = false;
    bool hasShark = false;
    bool isExit = false;
    bool isDHtoSWDoor = false; // for broken door in Observation Deck
    bool ghostDoorActive = false; // for Dining Hall ghost door trap
};

int main() {
    srand(time(0)); // RNG seed

    ProgramGreeting();

    string diverName;
    cout << "What's your name, diver? ";
    getline(cin, diverName);
    cout << "Welcome aboard, " << diverName << ".\n\n";

    // Room layout and descriptions
    // Ctl + Shit + L allows you to select all similar code when highlighting a specific piece of code (basically adds cursors to all like code)
    vector<Room> rooms = {
        {0, "Passenger Quarters", "Water logged bunk beds, the merky waters of lost hope.",
            "Water logged bunk beds, the merky waters of lost hope.", {"Knife"}},
        {1, "Passenger Quarters Stairwell", "A narrow passage. There are stairs going up and stairs going down.",
            "A narrow passage. There are stairs going up and stairs going down.", {}},
        {2, "Grand Ballroom", "Slimy kelp drappes over everything including the grand piano.",
            "Slimy kelp drappes over everything including the grand piano.", {"Metal Coffee Cup"}},
        {3, "Grand Ballroom Hallway", "Portraits line the walls. The sea water feels heavy and oily.",
            "Portraits line the walls. The sea water feels heavy and oily.", {}},
        {4, "Officer's Quarters", "The officer's uniforms float about. On a table: A splintered wooden map.",
            "The officer's uniforms float about. A splintered wooden map.", {"Map Etched on Rotted Wood", "Ancient Artifact"}},
        {5, "Ballast Hall", "It's cold and dark here but oddly you can hear the sound of a piano playing. Weird.",
            "It's cold and dark here but oddly you can hear the sound of a piano playing. Weird.", {}},
        {6, "Engine Room", "Echoing clangs of machinery. Shadows flicker near the gears.",
            "Echoing clangs of machinery. Shadows flicker near the gears.", {"Crowbar", "Nut and Bolt"}},
        {7, "Engine Room Stairwell", "Rusted pipes. A stairwell leads upward.",
            "Rusted pipes. A stairwell leads upward.", {}},
        {8, "Dining Hall", "Skeletal remains everywhere. A stairwell door to the north is barely hanging on its hinges.",
            "Skeletal remains everywhere. A stairwell door to the north is barely hanging on its hinges.", {}, -1,-1,-1,-1,-1,-1,false,false},
        {9, "Dining Hall Stairwell", "A final climb. Salt water rushes through the cracks.",
            "A final climb. Salt water rushes through the cracks.", {}},
        {10, "Observation Deck", "Slime and barnacles cover the glass. The surface world feels close.",
             "Slime and barnacles cover the glass. The surface world feels close.", {}, -1, -1, -1, -1, -1, -1, false, true},
        {11, "Observation Deck Stairwell", "A narrow stairwell. The gangsters block your way up unless you have the artifact.",
             "A narrow stairwell. The gangsters block your way up unless you have the artifact.", {}},
        {12, "Escape Boat", "The gangsters watch you closely. You need the artifact and map to escape.",
             "The gangsters watch you closely. You need the artifact and map to escape.", {}, -1, -1, -1, -1, -1, -1, true, false}
    };

    // Holding down Alt while selecting with the mouse allows manual multiple cursors
    // Drawing a flow chart on paper helped the most because this got confusing
    // Once I decided notto have the game flow in one direction that added a whole new level of complexity

    rooms[0].east = 1; rooms[1].west = 0; 
    rooms[1].up = 2; rooms[2].down = 1; 
    rooms[1].down = 5; rooms[5].up = 1; 
    rooms[2].east = 3; rooms[3].west = 2;
    rooms[3].north = 4; rooms[4].south = 3;
    rooms[5].south = 6; rooms[6].north = 5;
    rooms[6].east = 7; rooms[7].west = 6;
    rooms[7].up = 8; rooms[8].down = 7;
    rooms[8].east = 9; rooms[9].west = 8;
    rooms[9].up = 10; rooms[10].down = 9;
    rooms[10].east = 11; rooms[11].west = 10;
    rooms[11].up = 12; rooms[12].down = 11;

    // Press Ctl + D repeatedly to add cursors to the next identical piece of code
    // All hot-keys are for VS Code with Windows

    bool gameRunning = true;
    int currentRoom = 0;
    string input;
    bool DHDoorFixed = false; // observation deck door fixed with crowbar, nut and bolt
    bool roomLookedThisVisit = false;
    int previousRoom = -1;

    map<int, bool> roomLooked;    // Prevents player from being eaten randomly by shark spawn at no fault of their own
    map<int, bool> itemsRevealed; // if player revealed items in room like sharks or ghosts

    while (gameRunning) {
        Room& room = rooms[currentRoom];

        // Reset look-this-visit flag on room change
        if (currentRoom != previousRoom) {
            roomLookedThisVisit = false;
            previousRoom = currentRoom;
        }

        cout << "\nYou are in: " << room.name << endl;
        if (!room.visited) {
            cout << room.desc << endl;
            room.visited = true;
        } else {
            cout << room.reentryDesc << endl;
        }

        // Special Dining Hall ghost door logic, player must look first to see ghost trying to lock him in
        // if player does look then they can block door with cup from GB
        if (currentRoom == 8 && !roomLookedThisVisit) {
            room.ghostDoorActive = true;
        } else {
            room.ghostDoorActive = false;
        }

        // Special Observation Deck door fixing logic
        // Player must have the crowbar, nut and bolt to get through
        if (currentRoom == 10 && !DHDoorFixed) {
            cout << "\nThe door hinge here is broken. It won't open without fixing it first." << endl;
            if (hasItem("Crowbar") && hasItem("Nut and Bolt")) {
                cout << "You could fix the door with your Crowbar and Nut and Bolt. Fix now? (Y/N): ";
                getline(cin, input);
                if (!input.empty() && toupper(input[0]) == 'Y') {
                    cout << "You fix the door hinge with your tools. The path is now open." << endl;
                    DHDoorFixed = true;
                } else {
                    cout << "You decide not to fix the door right now." << endl;
                }
            } else {
                cout << "You need a Crowbar, Nut and Bolt to fix this door." << endl;
            }
        }

        // Check ghost door trap in Dining Hall
        if (room.ghostDoorActive) {
            cout << "If you don't look quickly, a ghost slams the door and traps you forever!" << endl;
        }

        cout << "\nCommand [N/S/E/W/U/D | L:Look | I:Inventory | Q:Quit]: ";
        getline(cin, input);
        if (input.empty()) continue;

        char cmd = toupper(input[0]);

        // Movement check before look: shark attack if shark present and not looked this visit (except PQ)
        // Shark can spawn randomly everywhere except PG because they don't have knife yet
        // Player must have knife and look first to survive random shark spawn
        auto isMoveCmd = [](char c) {
            return c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'U' || c == 'D';
        };
        if (room.hasShark && currentRoom != 0 && !roomLookedThisVisit && isMoveCmd(cmd)) {
            cout << "You didn't check your surroundings. As you move, a shark ambushes you!\nGAME OVER." << endl;
            break;
        }
        
        switch (cmd) { 
            case 'N':
                if (room.north != -1) currentRoom = room.north;
                else cout << "Can't go north." << endl;
                break;
            case 'S':
                if (room.south != -1) currentRoom = room.south;
                else cout << "Can't go south." << endl;
                break;
            case 'E':
                if (room.east != -1) currentRoom = room.east;
                else cout << "Can't go east." << endl;
                break;
            case 'W':
                if (room.west != -1) currentRoom = room.west;
                else cout << "Can't go west." << endl;
                break;
            case 'U':
                if (room.up != -1) currentRoom = room.up;
                else cout << "Can't go up." << endl;
                break;
            case 'D':
                if (room.down != -1) currentRoom = room.down;
                else cout << "Can't go down." << endl;
                break;
            case 'L': {

                // If in Dining Hall and ghost door active, looking blocks the door
                // No look then ghost slams doors player dies
                if (room.ghostDoorActive) {
                    cout << "You quickly look and spot a metal cup. You wedge it in the door to keep it from closing!" << endl;
                    room.ghostDoorActive = false;
                }

                // Reveal items if not already revealed
                if (room.items.empty() || itemsRevealed[room.id]) {
                    cout << "Nothing has changed about the room." << endl;
                } else {
                    cout << "You notice: ";
                    for (const string& item : room.items) cout << item << ", ";
                    cout << "\nPick up items? (Y/N): ";
                    getline(cin, input);
                    if (!input.empty() && toupper(input[0]) == 'Y') {
                        for (const string& item : room.items) {
                            inventory.push_back(item);
                            cout << "Picked up: " << item << endl;
                        }
                        room.items.clear();
                        itemsRevealed[room.id] = true;
                    } else {
                        cout << "You leave the items for now." << endl;
                    }
                }

                // Shark appears on look if present, if no look the player is eaten, so look in every room first thing
                if (room.hasShark) {
                    if (hasItem("Knife")) {
                        cout << "A shark appears! You stab it from under its mouth and into its brain.\n"
                             << "The shark flails and thrashes around then flips over belly up and dies" << endl;
                        room.hasShark = false;
                    } else {
                        cout << "A shark appears and you have nothing to defend yourself!\nGAME OVER." << endl;
                        gameRunning = false;
                    }
                }

                roomLooked[currentRoom] = true;
                roomLookedThisVisit = true;
                break;
            }
            case 'I': {
                cout << "\nInventory: ";
                if (inventory.empty()) cout << "Empty." << endl;
                else {
                    for (const string& item : inventory) cout << item << ", ";
                    cout << endl;
                }
                break;
            }
            case 'Q':
                cout << "You abandon the Sea Siren..." << endl;
                gameRunning = false;
                break;
            default:
                cout << "Invalid input." << endl;
        }

        // Check if player reached escape boat
        if (currentRoom == 12) {
            if (hasItem("Ancient Artifact") && hasItem("Map Etched on Rotted Wood")) {
                cout << "\nYou follow the map and artifact instructions and escape the Sea Siren!" << endl;
                gameRunning = false;
            } else { // Player must have map and artifact to exit
                cout << "\nThe gangsters won't let you escape without the artifact and map." << endl;
                currentRoom = 11; // Move player back to Observation Deck Stairwell
            }
        }
    }

    return 0;
}
