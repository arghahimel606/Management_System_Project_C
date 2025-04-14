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

#define ID_LEN 15 // lesser than allocated memory for display formatting
#define NAME_LEN 16
#define CATEGORY_LEN 13

typedef struct product product;

int prod_count = 0;

bool login(); // Function for login system, working using boolean values. Password Match = True, Else = False
void main_menu(); 
void view_all_product(); 
void view_product_by_category(); 
void add_product();
void search_product();         //// == temporary comments (for explaning task of unfinished functions)
//// create two sub functions, one for searching with a keyword that matches the first letter of the name of a product (tip = take a character input from user, match with the first letters of product name)
                               //// and another for matching string (tip: take a string input from user, match each character of the string with product name patrially with strncmp and using a loop);
                               //// can use display format like mine for outputting in terminal [like i used in view_product_by_category() function]
void edit_product();
//// use a search key (preferrably name string or ID string) to select the product to modify by matching(strcmp)
                               //// create functions or codes with ifelse/switchcase to access editing data elements like productID, name, category, price and quantity separately by user prompt or all by a separate conditon 
                               //// (e.g. user input 0 for editing all info and 1, 2, 3, 4, 5 for productID, name, category, price and quantity respectively)
                               //// use fseek() and ftell() [as necessary] function to access product info locations and modify file records
void delete_product(); 
//// take productID as input for matching string(strcmp) [deletes product info about specifically matching product ID] or take product name as input which will delete product info about every matching string (name) by partial search(strncmp)
                               //// Both separated by ifelse/switchcase as options
                               //// Tip: can create a separate function for partial search since it can be used in both search_product() and delete_product(), easier for reusability
void sales_management();
//// works with similar logic as delete function but will use product quantities' integer property for subtracting value after each successful trasaction according to number of quantities sold
                               //// e.g. lets say milk has a stock quantity of 5, after selling 3 amount [after user enters 3 as input for prompt] the value of milk's quantity would be updated to 2
                               //// also should display the amount of items sold (using temporary storing variables/ no need to record these in file) in terminal in a different UI view (displaying method)
                               //// e.g. after user enters a certain quantity of a product as sold, it shows as "3 Milk/s Have been successfully sold!" in terminal 
void logout(); 
void pause(); // Function to stop loops from doing instaneous output, instaneous output results in missing error messege (They are printed at incredible speed and loop continues)
void loading(); // Simple loading animation created by using ASCII characters
void loading_prod_count(); // Keeps track of prod_count from file
void save_prod_list(); // Saves product list info in the file
void limited_str_input(char *buffer, int size, const char *prompt); // Function to take limited user input for strings
                                                                    // used pointers to avoid hardcoding string size like buffer[20] in this function and to reference each string input correctly
                                                                    // stating size separately as parameter to use it in fgets  
                                                                    // also considered as best practice 
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
            //code
                break;
            case 4:
            //code
                break;
            case 5:
            //code
                break;
            case 6:
            //code
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
    printf("|  [4] Update Product            |\n"); // Editing Product
    printf("|  [5] Delete Product            |\n");
    printf("|  [6] Register Sales            |\n"); // Sales Management System
    printf("|  [0] Logout                    |\n");
    printf("==================================\n");
    printf(">> Enter Your Choice: ");
}

void view_all_product()
{
    system(CLEAR);

    FILE *fp;

    fp = fopen("product_list.bin", "rb"); // rb = mode for reading bin(binary) file

    if(fp == NULL)
    {
        printf("Could not find product list info");
        pause();
        return;
    }
    else
    {
        if(prod_count == 0)
        {
            printf("No product available at the moment.");
            pause();
            return;
        }

        loading();
        int i;
            printf("=============================================================================================================\n");
            printf("   Serial No.   |     Product ID     |     Product Name    |     Category     |    Price    |    Quantity    \n");
            printf("=============================================================================================================\n");
        int temp = fread(&prod, sizeof(product), prod_count, fp);
        for(i = 0; i < temp; i++)
        {
            printf("  %-14d|   %-17s|   %-18s|   %-15s|  %-10.2lf$|      %-14d\n", i+1, prod[i].productID, prod[i].name, prod[i].category, prod[i].price, prod[i].quantity);
        }
        fclose(fp);
    }

    pause();
}

void view_product_by_category()
{
    system(CLEAR);

    FILE *fp;

    fp = fopen("product_list.bin", "rb"); 

    if(fp == NULL)
    {
        printf("Could not find product list info");
        pause();
        return;
    }
    else
    {
        if(prod_count == 0)
        {
            printf("No product available at the moment.");
            pause();
            return;
        }

        char input_category[20];

        limited_str_input(input_category, 13, "Enter Product Category: ");

        system(CLEAR);

        loading();

        int i, found = 0;

        fclose(fp);

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
        
    }

    pause();
}

void add_product()
{
    system(CLEAR);

    FILE *fp;

    fp = fopen("product_list.bin", "ab"); // ab = mode for appending bin(binary) file

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

        loading();

        printf("\nProduct added successfully!");
        
        pause();

        fclose(fp);
    }
}

void search_product()
{

}

void edit_product()
{

}

void delete_product()
{

}

void sales_management()
{

}

void logout()
{
    system(CLEAR);
    loading();
    printf("\rLogout Successful! ");
}

void pause()
{
    printf("\nPress Enter to continue...");
    while(getchar() != '\n');
}

void loading()
{
    const char frames[] = "|/-\\";
    int frame_count = 4;

    

    for (int i = 0; i < 40; i++) {
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
