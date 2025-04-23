#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32 //  _WIN32 for windows set of executions, else for unix operating systems (e.g. Linux or Mac)
  #include <windows.h>
  #define CLEAR "cls"
  #define SLEEP(ms) Sleep(ms)
#else
  #include <unistd.h>
  #define CLEAR "clear"
  #define SLEEP(ms) usleep((ms) * 1000) // converting function parameter [Sleep uses milisecond (windows), usleep uses microseconds (for unix based systems)]
#endif

#define MAX_PRODUCT 100

struct product 
{
    char productID[20];
    char name[20];
    char category[20];
    double price;
    int quantity;
} prod[MAX_PRODUCT];

#define ID_LEN 15 // lesser than allocated memory for adjusting display formatting
#define NAME_LEN 16
#define CATEGORY_LEN 13

typedef struct product product;

int prod_count = 0;

bool login(); // Function for login system, working using boolean values. Password Match = True, Else = False
void main_menu(); 
void view_all_product(); 
void view_product_by_category(); 
void add_product();
void search_product();
void edit_product();
void delete_product(); 
void sales_management();
void logout(); 
void pause(); // Function to stop loops from doing instaneous output, instaneous output results in missing error messege (They are printed at incredible speed and loop continues)
void loading(int time); // Simple loading animation created by using ASCII characters
void loading_prod_count(); // Keeps track of prod_count from file
void save_prod_list(); // Saves product list info in the file
void limited_str_input(char *buffer, int size, const char *prompt); // Self-developed function to limit character inputs for strings
                                                                    // used pointers to reference each string input correctly
                                                                    // stating size separately as parameter to use it in fgets  
int main()
{
    while(!login())
    {
        printf("Wrong password, try again.");
    }

    loading_prod_count();

    int option, viewop; // viewop is a variable to store input from user for suboption for viewing productlist by all or category

    do
    {
        while(1)
        {
            main_menu();
            if(scanf("%d", &option) != 1) // Safeguarding scanf from non integer inputs (scanf returns 1 for a integer input)
            {
                while(getchar() != '\n'); // Consuming characters before newline in buffer
                printf("\nInvalid choice. Please enter a number");
                pause();
                continue;
            }
            getchar(); // To consume newline
            break;
        }
        switch(option)
        {
            case 0:
            save_prod_list(); // saves product info from ram to storage(file) before exiting [crucial for keeping changes and updates saved]
            logout();
            return 0;
                break;
            case 1:
            do
            {
                system(CLEAR);
                printf("[1] View All Products\n[2] View Products by Category\n");
                printf("\n>> Enter Your Choice: ");
                if(scanf("%d", &viewop) != 1)
                {
                    while(getchar() != '\n');
                    printf("\nInvalid choice. Please enter a number");
                    pause();
                    continue;
                }
                getchar();
                if(viewop == 1)
                {
                    view_all_product();
                }
                else if(viewop == 2)
                {
                    view_product_by_category();
                }
                else
                {
                    printf("Invalid choice, try again.\n");
                    pause();
                    getchar();
                }
            } while(viewop != 1 && viewop != 2);
                break;
            case 2:
            add_product();
                break;
            case 3:
            search_product();
                break;
            case 4:
            edit_product();
                break;
            case 5:
            delete_product();
                break;
            case 6:
            sales_management();
                break;
            default:
                printf("Invalid choice, try again.\n");
                pause();
                continue;
        }

    }while(option != 0);
    
    return 0;
}

bool login()
{
    char password[20];
    
    limited_str_input(password, 19, "Enter Password: ");

    for(int i = 0; i < strlen(password); i++)
    {
        password[i] = tolower(password[i]); // removing case sensitivity
    }

    if(strcmp(password, "admin123") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void main_menu()
{
    system(CLEAR);

    printf("\n");
    printf("==================================\n");
    printf("|          MAIN MENU             |\n");
    printf("==================================\n");
    printf("|  [1] View Avaliable Products   |\n");
    printf("|  [2] Add New Product           |\n");
    printf("|  [3] Search Product            |\n");
    printf("|  [4] Update Product            |\n"); 
    printf("|  [5] Delete Product            |\n");
    printf("|  [6] Register Sales            |\n"); 
    printf("|  [0] Logout                    |\n");
    printf("==================================\n");
    printf(">> Enter Your Choice: ");
}

void view_all_product()
{
    system(CLEAR);

        if(prod_count == 0)
        {
            printf("No product available at the moment.");
            pause();
            return;
        }

        loading(40);
        int i;
            printf("=============================================================================================================\n");
            printf("   Serial No.   |     Product ID     |     Product Name    |     Category     |    Price    |    Quantity    \n");
            printf("=============================================================================================================\n");
        
        for(i = 0; i < prod_count; i++)
        {
            printf("  %-14d|   %-17s|   %-18s|   %-15s|  %-10.2lf$|      %-14d\n", i+1, prod[i].productID, prod[i].name, prod[i].category, prod[i].price, prod[i].quantity);
        }

    pause();
}

void view_product_by_category()
{
    system(CLEAR);

        if(prod_count == 0)
        {
            printf("No product available at the moment.");
            pause();
            return;
        }

        char input_category[20];

        limited_str_input(input_category, 13, "Enter Product Category: ");

        system(CLEAR);

        loading(30);

        int i, found = 0;

        printf("\n                               Products Under %s Category\n", input_category);
            printf("=============================================================================================================\n");
            printf("   Serial No.   |     Product ID     |     Product Name    |     Category     |    Price    |    Quantity    \n");
            printf("=============================================================================================================\n");

        for(i = 0; i < strlen(input_category); i++)
        {
            input_category[i] = tolower(input_category[i]);
        }

        for(i = 0; i < prod_count; i++)
        {
            char lower_category[20];
            strcpy(lower_category, prod[i].category); 
            for(int j = 0; j < strlen(lower_category); j++)
            {
                lower_category[j] = tolower(lower_category[j]);
            }

            if(strcmp(lower_category, input_category) == 0)
            {
                found++;

                printf("  %-14d|   %-17s|   %-18s|   %-15s|  %-10.2lf$|      %-14d\n", i+1, prod[i].productID, prod[i].name, prod[i].category, prod[i].price, prod[i].quantity);

            }

        }

           if(found == 0)
           {
                printf("                            No Products Found in this category\n");
           } 

    pause();
}

void add_product()
{
    system(CLEAR);

    FILE *fp;

    fp = fopen("product_list.bin", "ab"); 

    if(fp == NULL)
    {
        printf("Product list info does not exist, please add to create one");
        pause();
        exit(1);   
    }
    else
    {
        if(prod_count >= MAX_PRODUCT)
        {
            printf("Inventory full, cannot add more.");
            pause();
            return; 
        }

        limited_str_input(prod[prod_count].productID, ID_LEN, "Enter product ID: ");

        for(int i = 0; i < prod_count; i++)
        {
            if(strcmp(prod[i].productID, prod[prod_count].productID) == 0) // prevents entering duplicate product ID
            {
                printf("Product ID already exists!\n");
                pause();
                fclose(fp);
                return;
            }
        }

        limited_str_input(prod[prod_count].name, NAME_LEN, "Enter product name: ");
        limited_str_input(prod[prod_count].category, CATEGORY_LEN, "Enter product category: ");
        
        while(1)
        {
            printf("Enter product price: ");
            if(scanf("%lf", &prod[prod_count].price) != 1 || prod[prod_count].price < 0) // reprompting the user for input when a non-positive integer or non-integer is entered
            {
                while(getchar() != '\n');
                printf("\nInvalid price. Enter again.");
                pause();
                continue;
            }
            getchar();
            break;
        }
        
        while(1)
        {
            printf("Enter quantity to add: ");
            if(scanf("%d", &prod[prod_count].quantity) != 1 || prod[prod_count].quantity < 0)
            {
                while(getchar() != '\n');
                printf("\nInvalid quantity. Enter again.");
                pause();
                continue;
            }
            getchar();
            break;
        }
        fwrite(&prod[prod_count], sizeof(product), 1, fp);

        prod_count++;

        loading(20);

        printf("\nProduct added successfully!");
        
        pause();

        fclose(fp);
    }
}

void search_product()
{
    system(CLEAR);
    int choice;

    printf("[1] Search by First Letter\n[2] Search by Matching Keyword\n>> Enter choice: ");
    scanf("%d", &choice);
    getchar();

    switch(choice)
    {
        case 1:
            system(CLEAR);
            int i, found = 0;
            char letter;
            printf("\nEnter First Letter: ");
            scanf(" %c", &letter);
            getchar();

            if(toupper(letter) < 'A' || toupper(letter) > 'Z')
            {
                printf("Invalid input, try again.");
                pause();
                return;
            }
            

            loading(30);
            
            printf("=============================================================================================================\n");
            printf("   Serial No.   |     Product ID     |     Product Name    |     Category     |    Price    |    Quantity    \n");
            printf("=============================================================================================================\n");
        
            for(i = 0; i < prod_count; i++)
            {
                if(tolower(letter) == tolower(prod[i].name[0]))
                {
                    found++;
                    printf("  %-14d|   %-17s|   %-18s|   %-15s|  %-10.2lf$|      %-14d\n", i+1, prod[i].productID, prod[i].name, prod[i].category, prod[i].price, prod[i].quantity);
                }
            }

            if(found == 0)
            {
                printf("                             No Products Found that start with \'%c\'\n", letter);
            } 

            pause();

        break;

        case 2:
            system(CLEAR);
            int j, matched = 0;
            char keyword[NAME_LEN];

            limited_str_input(keyword, NAME_LEN, "\nEnter keyword: ");

            loading(30);
        
            printf("\n                                      Matching Products\n");
            printf("=============================================================================================================\n");
            printf("   Serial No.   |     Product ID     |     Product Name    |     Category     |    Price    |    Quantity    \n");
            printf("=============================================================================================================\n");

            for(i = 0; i < prod_count; i++)
            {
                if(strncasecmp(keyword, prod[i].name, strlen(keyword)) == 0)
                {
                    matched++;
                    printf("  %-14d|   %-17s|   %-18s|   %-15s|  %-10.2lf$|      %-14d\n", i+1, prod[i].productID, prod[i].name, prod[i].category, prod[i].price, prod[i].quantity);
                }
            }

            if(matched == 0)
            {
                printf("                                      No Products Found\n");
            } 

            pause();

        break;

        default:
            printf("Invalid option!\n");
        pause();
        return;

    }
}

void edit_product()
{
    system(CLEAR);
    
    char keyword[20];
    limited_str_input(keyword, 20, "Enter Product ID or Name to search: ");

    int index = -1;
    for (int i = 0; i < prod_count; i++)
    {
        if (strcmp(prod[i].productID, keyword) == 0 || strcasecmp(prod[i].name, keyword) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("\nProduct not found!\n");
        pause();
        return;
    }
    loading(20);
    printf("\nProduct Found:\n");
    printf("ID: %s\nName: %s\nCategory: %s\nPrice: %.2lf$\nQuantity: %d\n",
           prod[index].productID, prod[index].name, prod[index].category,
           prod[index].price, prod[index].quantity);

    printf("\nDo you want to edit this product? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if (tolower(confirm) != 'y')
    {
        printf("Returning to main menu...\n");
        pause();
        return;
    }

    int field;
    printf("\nSelect field to edit:\n");
    printf("[0] Edit All\n[1] ID\n[2] Name\n[3] Category\n[4] Price\n[5] Quantity\n");
    printf(">> Your choice: ");
    scanf("%d", &field);
    getchar();

    switch (field)
    {
    case 0:
        limited_str_input(prod[index].productID, ID_LEN, "New ID: ");
        limited_str_input(prod[index].name, NAME_LEN, "New Name: ");
        limited_str_input(prod[index].category, CATEGORY_LEN, "New Category: ");
        printf("New Price: ");
        scanf("%lf", &prod[index].price);
        getchar();
        printf("New Quantity: ");
        scanf("%d", &prod[index].quantity);
        getchar();
        break;

    case 1:
        limited_str_input(prod[index].productID, ID_LEN, "New ID: ");
        break;

    case 2:
        limited_str_input(prod[index].name, NAME_LEN, "New Name: ");
        break;

    case 3:
        limited_str_input(prod[index].category, CATEGORY_LEN, "New Category: ");
        break;

    case 4:
        printf("New Price: ");
        scanf("%lf", &prod[index].price);
        getchar();
        break;

    case 5:
        printf("New Quantity: ");
        scanf("%d", &prod[index].quantity);
        getchar();
        break;

    default:
        printf("Invalid option!\n");
        pause();
        return;
    }

    save_prod_list();
    loading(30);
    printf("\nProduct updated successfully!\n");
    pause();
}

void delete_product()
{
    system(CLEAR);
    int choice;
    
    // Ask user how they want to delete a product
    printf("[1] Delete by Product ID\n[2] Delete by Partial Name\n>> Enter choice: ");
    scanf("%d", &choice);
    getchar();  

    char input[20];  // For user input (ID or name)
    int deleted = 0; // Count how many products are deleted

    if (choice == 1) {
        // Delete by exact Product ID
        limited_str_input(input, 20, "Enter Product ID: ");

        for (int i = 0; i < prod_count; i++) {
            if (strcmp(prod[i].productID, input) == 0) {
                // Shift all items after the deleted product to the left
                for (int j = i; j < prod_count - 1; j++) {
                    prod[j] = prod[j + 1];
                }
                prod_count--;  // Reduce total product count
                deleted++;     // Mark as deleted
                break;         // Exit loop after deletion
            }
        }

    } else if (choice == 2) {
        // Delete all products whose names match a partial string
        limited_str_input(input, 20, "Enter partial product name: ");

        for (int i = 0; i < prod_count;) {
            if (strncasecmp(prod[i].name, input, strlen(input)) == 0) {
                // Found a match, delete it by shifting
                for (int j = i; j < prod_count - 1; j++) {
                    prod[j] = prod[j + 1];
                }
                prod_count--;
                deleted++;
                // Do not increment i, as the next product has shifted into current index
            } else {
                i++;
            }
        }

    } else {
        printf("Invalid choice!\n");
        pause();
        return;
    }

    // Save updated product list to file
    save_prod_list();

    if (deleted)
    {
        loading(20);
        printf("%d product(s) deleted successfully.\n", deleted);
    }        
    else
        printf("No matching products found to delete.\n");

    pause();  // Wait for user before clearing the screen
}

void sales_management()
{
    system(CLEAR);
    char id[20];
    int quantity;

    // Get Product ID from user
    limited_str_input(id, 20, "Enter Product ID to sell: ");

    int found = -1;

    // Search for the product by ID
    for (int i = 0; i < prod_count; i++) {
        if (strcmp(prod[i].productID, id) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        // Product not found
        printf("Product not found!\n");
        pause();
        return;
    }

    // Ask how many units to sell
    printf("Enter quantity to sell: ");
    scanf("%d", &quantity);
    getchar();

    // Check if requested quantity is valid
    if (quantity <= 0 || quantity > prod[found].quantity) {
        printf("Invalid quantity. Only %d in stock.\n", prod[found].quantity);
        pause();
        return;
    }

    // Reduce product quantity after sale
    prod[found].quantity -= quantity;

    // Save updated product list to file
    save_prod_list();

    loading(20);

    // Confirmation message
    printf("\n%d %s/s have been successfully sold!\n", quantity, prod[found].name);
    pause();
}

void logout()
{
    system(CLEAR);
    loading(30);
    printf("\rLogout Successful! ");
}

void pause()
{
    printf("\nPress Enter to continue...");
    while(getchar() != '\n');
}

void loading(int time) //time*10 seconds
{
    const char frames[] = "|/-\\";
    int frame_count = 4;

    

    for (int i = 0; i < time; i++) {
        printf("\r %c", frames[i % frame_count]);
        fflush(stdout);
        SLEEP(100);
    }
    
    system(CLEAR);
}

void loading_prod_count()
{
    FILE *fp = fopen("product_list.bin", "rb");

    if(fp == NULL)
    {
        printf("Failed to load product list data");
        pause();
        return;
    }
    else
    {
        prod_count = fread(prod, sizeof(product), MAX_PRODUCT, fp);

        fclose(fp);
    }
}

void save_prod_list()
{
    FILE *fp = fopen("product_list.bin", "wb");

    if(fp == NULL)
    {
        printf("Failed to save product data");
        pause();
        exit(1);
    }
    else
    {
        fwrite(prod, sizeof(product), prod_count, fp);

        fclose(fp);
    }
}

void limited_str_input(char *buffer, int size, const char *prompt)
{
    int flush;

    while(1)
    {
        printf("%s", prompt);
        fgets(buffer, size, stdin); // fgets instead of gets for safer input

        if(buffer[strlen(buffer) - 1] != '\n')
        {
            while((flush = getchar()) != '\n' && flush != EOF); // loop to remove exceesive characters until newline or EOF from buffer to prevent skipping next prompt 
            printf("Input exceeds word limit. Please input %d characters or less.\n", size - 1);
        }
        else
        {
            buffer[strcspn(buffer, "\n")] = '\0'; // replacing the newline with null terminator at the end of the string assigned by fgets instead of clearing buffer with fflush(does same job)+(works on multi-platform)
            break;
        }
    }
}
