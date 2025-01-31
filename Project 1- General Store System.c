#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 10
#define MAX_USERS 100  // Increased to handle more users
#define USER_FILE "users.txt"  // File to store user login details
#define CART_FILE "carts.txt"  // File to store user cart details

// Struct for storing user details
typedef struct {
    char username[20];
    char password[20];
} User;

// Struct for storing item details
typedef struct {
    char name[50];
    float price;  // Price per kg
    float quantity; // Quantity in kg
} Item;

// Array of items
Item items[MAX_ITEMS] = {
    {"Apple", 3.0, 0},
    {"Banana", 1.5, 0},
    {"Rice", 2.0, 0},
    {"Wheat", 1.8, 0},
    {"Milk", 1.2, 0},
    {"Cheese", 4.5, 0},
    {"Tomato", 2.5, 0},
    {"Potato", 1.0, 0},
    {"Chicken", 5.0, 0},
    {"Carrot", 6.0, 0}
};

// Function to sign up a new user and save details to file
int signUp() {
    FILE *file = fopen(USER_FILE, "a");  // Open in append mode
    if (file == NULL) {
        printf("Error opening file for saving user data.\n");
        return 0;
    }

    char username[20], password[20];
    printf("Enter a new username: ");
    scanf("%19s", username);  // Limit input to prevent buffer overflow
    printf("Enter a new password: ");
    scanf("%19s", password);

    // Save the new user's details to the file
    fprintf(file, "%s %s\n", username, password);
    fclose(file);
    printf("Sign-up successful!\n");
    return 1;
}

// Function to load user details from file for authentication
int loadUsers(User users[], int *num_users) {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        printf("No users found. Please sign up first.\n");
        return 0;
    }

    *num_users = 0;  // Reset the user count
    while (fscanf(file, "%19s %19s", users[*num_users].username, users[*num_users].password) != EOF) {
        (*num_users)++;
    }

    fclose(file);
    return 1;
}

// Function to authenticate user login
int login(User users[], int num_users, char *current_user) {
    char username[20], password[20];
    printf("Enter username: ");
    scanf("%19s", username);
    printf("Enter password: ");
    scanf("%19s", password);

    // Check if the entered username and password match any saved user data
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            strcpy(current_user, username); // Store the logged-in username
            return 1; // Login successful
        }
    }
    return 0; // Login failed
}

// Function to display available items
void displayItems() {
    printf("\nItems available for shopping:\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
        printf("%d. %s (Price: %.2f per kg)\n", i + 1, items[i].name, items[i].price);
    }
}

// Function to display the cart with improved alignment
void displayCart() {
    float total = 0;
    printf("\nYour Cart:\n");
    printf("%-15s %-15s %-15s %-15s\n", "Item Name", "Quantity (kg)", "Price (per kg)", "Total");

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].quantity > 0) {
            float item_total = items[i].quantity * items[i].price;
            total += item_total;
            printf("%-15s %-15.2f %-15.2f %-15.2f\n", items[i].name, items[i].quantity, items[i].price, item_total);
        }
    }

    printf("\nTotal Amount in Cart: %.2f\n", total);
}

// Function to save the cart and bill to the cart file
void saveCartToFile(char *username) {
    FILE *file = fopen(CART_FILE, "a");
    if (file == NULL) {
        printf("Error opening file for saving cart details.\n");
        return;
    }

    float total = 0;
    fprintf(file, "\nUser: %s's Cart\n", username);
    fprintf(file, "%-15s %-15s %-15s %-15s\n", "Item Name", "Quantity (kg)", "Price (per kg)", "Total");

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].quantity > 0) {
            float item_total = items[i].quantity * items[i].price;
            total += item_total;
            fprintf(file, "%-15s %-15.2f %-15.2f %-15.2f\n", items[i].name, items[i].quantity, items[i].price, item_total);
        }
    }

    fprintf(file, "\nTotal Bill: %.2f\n", total);
    fclose(file);
    printf("\nCart details saved to '%s'.\n", CART_FILE);
}

// Function to generate the final bill
void generateBill(char *username) {
    printf("\nYour Shopping Bill:\n");
    displayCart();

    // Save the bill and cart to the file
    saveCartToFile(username);

    // Clear the cart after generating the bill
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].quantity = 0;
    }
}

// Function to update the cart
void updateCart() {
    int choice;
    while (1) {
        displayCart();
        printf("\n1. Add Item\n2. Remove Item\n3. Update Quantity\n4. Exit\nChoose an action: ");
        scanf("%d", &choice);

        if (choice == 4) {
            break;
        } else if (choice == 1) {
            int item_choice;
            float quantity;
            displayItems();
            printf("\nSelect item number to add to cart: ");
            scanf("%d", &item_choice);

            if (item_choice > 0 && item_choice <= MAX_ITEMS) {
                printf("Enter quantity in kg for %s: ", items[item_choice - 1].name);
                scanf("%f", &quantity);
                items[item_choice - 1].quantity += quantity;
            } else {
                printf("Invalid item choice.\n");
            }
        } else if (choice == 2) {
            int item_choice;
            displayCart();
            printf("\nSelect item number to remove from cart: ");
            scanf("%d", &item_choice);

            if (item_choice > 0 && item_choice <= MAX_ITEMS && items[item_choice - 1].quantity > 0) {
                items[item_choice - 1].quantity = 0;
                printf("Item removed from cart.\n");
            } else {
                printf("Invalid item choice or item not in cart.\n");
            }
        } else if (choice == 3) {
            int item_choice;
            float quantity;
            displayCart();
            printf("\nSelect item number to update quantity: ");
            scanf("%d", &item_choice);

            if (item_choice > 0 && item_choice <= MAX_ITEMS && items[item_choice - 1].quantity > 0) {
                printf("Enter new quantity in kg for %s: ", items[item_choice - 1].name);
                scanf("%f", &quantity);
                items[item_choice - 1].quantity = quantity;
            } else {
                printf("Invalid item choice or item not in cart.\n");
            }
        } else {
            printf("Invalid choice.\n");
        }
    }
}

int main() {
    User users[MAX_USERS];
    int num_users = 0;
    int loggedIn = 0, choice, action;
    char current_user[20];

    printf("Welcome to our general store \n");

    // Load users from file into the program's memory
    if (!loadUsers(users, &num_users)) {
        printf("No users found. Please sign up first.\n");
        if (!signUp()) {
            return 1; // Exit if sign-up fails
        }
        // Reload users after sign-up
        num_users = 0;
        if (!loadUsers(users, &num_users)) {
            printf("Error loading user data after sign-up.\n");
            return 1;
        }
    }

    // Main loop to allow multiple logins and sign-ups
    while (1) {
        printf("\n1. Login\n2. Sign Up\n3. Exit\nChoose an action: ");
        scanf("%d", &action);

        if (action == 3) {
            break; // Exit the program
        } else if (action == 1) {
            // User login process
            loggedIn = 0;
            while (!loggedIn) {
                loggedIn = login(users, num_users, current_user);
                if (!loggedIn) {
                    printf("Invalid login details. Try again.\n");
                } else {
                    printf("Login successful! Welcome to the Shopping Cart.\n");

                    // Main shopping loop
                    while (1) {
                        displayItems();
                        printf("\nSelect item number to add to cart (1-10, 0 to finish): ");
                        scanf("%d", &choice);

                        if (choice == 0) {
                            break; // Exit loop to manage cart
                        } else if (choice > 0 && choice <= MAX_ITEMS) {
                            float quantity;
                            printf("Enter quantity in kg for %s: ", items[choice - 1].name);
                            scanf("%f", &quantity);

                            items[choice - 1].quantity += quantity;

                            // Display the cart after adding the item
                            displayCart();
                        } else {
                            printf("Invalid choice. Please try again.\n");
                        }
                    }

                    // Allow user to update the cart before generating the bill
                    updateCart();

                    // Generate the final bill
                    generateBill(current_user);
                }
            }
        } else if (action == 2) {
            // Sign up new user
            if (!signUp()) {
                return 1; // Exit if sign-up fails
            }
            // Reload users after sign-up
            num_users = 0;
            if (!loadUsers(users, &num_users)) {
                printf("Error loading user data after sign-up.\n");
                return 1;
            }
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
