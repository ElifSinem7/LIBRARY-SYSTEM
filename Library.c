#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void CreateCustomer();
void DepositMoney();
void AddBook();
void RentBook();
void DeliveryBook();
void BurnBook();
void UpdateCustomer_Information();
void UpdateBook_Information();
void List_RentingCustomers();
void List_OfCustomers();
void List_OfBooks();

typedef struct Customer {
    int C_ID;
    char name[50], surname[50];
    int age;
    double wallet;
} Customer;

typedef struct Book {
    int B_ID;
    char name[50], author[50];
    int age_limit;
    double price_per_week;
    int rented;
} Book;

typedef struct Rented {
    int R_ID, C_ID, B_ID;
    char rented_date[20];
    int week;
} Rented;

FILE *customers;
FILE *books;
FILE *rented;

int getNewRentId() { // This function gives ID for the rented books. It starts from 1 and keeps going while we adding new rented books.
    FILE *file = fopen("rented.txt", "r");
    if (file == NULL) return 1;
    int maxID = 0, id;
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d", &id);
        if (id > maxID) maxID = id;
    }
    fclose(file);
    return maxID + 1;
}
int getNewBookId(){ // This function gives ID for the books. It starts from 1 and keeps going while we adding new books to the library.
    FILE *file = fopen("books.txt", "r");
    if(file == NULL) return 1;
    int maxID = 0, id;
    char line[200];
    while(fgets(line, sizeof(line), file)){
        sscanf(line, "%d", &id);
        if(id > maxID) maxID = id;
    }
    fclose(file);
    return maxID + 1;
}

Customer* getCustomerById(int C_ID) { // This function helps us to find a customer by entering her/his ID.
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL) return NULL;
    Customer *customer = (Customer*) malloc(sizeof(Customer));
    while (fscanf(file, "%d %s %s %d %lf", &customer->C_ID, customer->name, customer->surname, &customer->age, &customer->wallet) != EOF) {
        if (customer->C_ID == C_ID) {
            fclose(file);
            return customer;
        }
    }
    fclose(file);
    free(customer);
    return NULL;
}

Book* getBookById(int B_ID) { // This function helps us to find a book by using its ID.
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) return NULL;
    Book *book = (Book*) malloc(sizeof(Book));
    while (fscanf(file, "%d %s %s %d %lf %d", &book->B_ID, book->name, book->author, &book->age_limit, &book->price_per_week, &book->rented) != EOF) {
        if (book->B_ID == B_ID) {
            fclose(file);
            return book;
        }
    }
    fclose(file);
    free(book);
    return NULL;
}

void updateCustomerFile(Customer* customer) { // This function updates the customer file. If the customer exist in the file it stays same but if the customer doesn't exist in the file, it creates this customer as a new customer.
    FILE *file = fopen("customers.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    Customer tempCustomer;

    while (fscanf(file, "%d %s %s %d %lf", &tempCustomer.C_ID, tempCustomer.name, tempCustomer.surname, &tempCustomer.age, &tempCustomer.wallet) != EOF) {
        if (tempCustomer.C_ID == customer->C_ID) {
            fprintf(temp, "%d %s %s %d %lf\n", customer->C_ID, customer->name, customer->surname, customer->age, customer->wallet);
        } else {
            fprintf(temp, "%d %s %s %d %lf\n", tempCustomer.C_ID, tempCustomer.name, tempCustomer.surname, tempCustomer.age, tempCustomer.wallet);
        }
    }
    fclose(file);
    fclose(temp);
    remove("customers.txt");
    rename("temp.txt", "customers.txt");
}

void updateBookFile(Book* book) { // This function updates the book file. If the book exist in the file it stays the samebut if it doesn't exist in the file, it creates this book as a new book.
    FILE *file = fopen("books.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    Book tempBook;

    while (fscanf(file, "%d %s %s %d %lf %d", &tempBook.B_ID, tempBook.name, tempBook.author, &tempBook.age_limit, &tempBook.price_per_week, &tempBook.rented) != EOF) {
        if (tempBook.B_ID == book->B_ID) {
            fprintf(temp, "%d %s %s %d %lf %d\n", book->B_ID, book->name, book->author, book->age_limit, book->price_per_week, book->rented);
        } else {
            fprintf(temp, "%d %s %s %d %lf %d\n", tempBook.B_ID, tempBook.name, tempBook.author, tempBook.age_limit, tempBook.price_per_week, tempBook.rented);
        }
    }
    fclose(file);
    fclose(temp);
    remove("books.txt");
    rename("temp.txt", "books.txt");
}

void updateRentFile(Rented* rent) { // The updateRentFile function updates a specific Rented record by finding the appropriate record in the rented.txt file and updating it.
    FILE *file = fopen("rented.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    Rented tempRent;

    while (fscanf(file, "%d %d %d %s %d", &tempRent.R_ID, &tempRent.C_ID, &tempRent.B_ID, tempRent.rented_date, &tempRent.week) != EOF) {
        if (tempRent.R_ID == rent->R_ID) {
            fprintf(temp, "%d %d %d %s %d\n", rent->R_ID, rent->C_ID, rent->B_ID, rent->rented_date, rent->week);
        } else {
            fprintf(temp, "%d %d %d %s %d\n", tempRent.R_ID, tempRent.C_ID, tempRent.B_ID, tempRent.rented_date, tempRent.week);
        }
    }
    fclose(file);
    fclose(temp);
    remove("rented.txt");
    rename("temp.txt", "rented.txt");
}

int getNewCustomerId() { // This function ensures that each added customer is given an ID. It starts to give ID's from 1 and keep going.
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL) return 1;
    int maxID = 0, id;
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d", &id);
        if (id > maxID) maxID = id;
    }
    fclose(file);
    return maxID + 1;
}

void CreateCustomer() { // This function creates customer. Users enter their name, surname, age and wallet value then getNewCustomerId function gives them an ID.
    Customer customer;
    customers = fopen("customers.txt", "a+");
    if (customers == NULL) {
        printf("Error opening file!\n");
        return;
    }
    customer.C_ID = getNewCustomerId();
    printf("Enter the Name of the Customer: \n");
    scanf("%s", customer.name);
    printf("Enter the Surname of the Customer: \n");
    scanf("%s", customer.surname);
    printf("Enter the Age of the Customer: \n");
    scanf("%d", &customer.age);
    printf("Enter the Wallet value: \n");
    scanf("%lf", &customer.wallet);
    fprintf(customers, "%d %s %s %d %lf\n", customer.C_ID, customer.name, customer.surname, customer.age, customer.wallet);
    fclose(customers);
    printf("(' ') A New Customer has been added successfully with ID: %d\n", customer.C_ID);
}

void DepositMoney() { // This function helps to add money to the customers wallet.
    int C_ID, amount;
    printf("Enter customer ID: ");
    scanf("%d", &C_ID);
    printf("Enter deposit amount: ");
    scanf("%d", &amount);

    Customer* customer = getCustomerById(C_ID);
    if (customer == NULL) {
        printf("Customer not found!\n");
        return;
    }
    customer->wallet += amount;
    updateCustomerFile(customer);

    printf("Money deposited successfully. New wallet balance: %.2lf\n", customer->wallet);
    free(customer);
}

void AddBook() {// This function adds book to the library. Users enter the name of the book, author, age limit and the price per week to use the book. 
    Book book;
    books = fopen("books.txt", "a+");
    if (books == NULL) {
        printf("Error opening file!\n");
        return;
    }
    book.B_ID = getNewBookId();
    printf("Enter the Name of the Book: \n");
    scanf("%s", book.name);
    printf("Enter the Name of the Author: \n");
    scanf("%s", book.author);
    printf("Enter the Age Limit of the Book: \n");
    scanf("%d", &book.age_limit);
    printf("Enter the Weekly Price: \n");
    scanf("%lf", &book.price_per_week);
    book.rented = 0;
    fprintf(books, "%d %s %s %d %lf %d\n", book.B_ID, book.name, book.author, book.age_limit, book.price_per_week, book.rented);
    fclose(books);
    printf("(' ') A New Book has been added successfully with ID: %d\n", book.B_ID);
}

void RentBook() { // This function helps to people to rent the books. Users enter their ID , the book which they want to take ID and the weeks they want to use the book. If book rented the system will say "Book is already rented!" or if yo don't have enough money to take the book system will say "You don't have enough money!"
    int C_ID, B_ID, week;
    printf("Enter Customer ID: ");
    scanf("%d", &C_ID);
    printf("Enter Book ID: ");
    scanf("%d", &B_ID);
    printf("Enter rental duration in weeks: ");
    scanf("%d", &week);

    Customer* customer = getCustomerById(C_ID);
    Book* book = getBookById(B_ID);

    if (customer == NULL) {
        printf("Customer not found!\n");
        return;
    }
    if (book == NULL) {
        printf("Book not found!\n");
        return;
    }
    if (book->rented) {
        printf("Book is already rented!\n");
        return;
    }
    if(book->age_limit > customer->age){
        printf("You are under the book's age limit. So you cannot rent the book.");
    }
    double totalCost = book->price_per_week * week;
    if (customer->wallet < totalCost) {
        printf("You don't have enough money!\n");
        return;
    }
    customer->wallet -= totalCost;
    book->rented = 1;
    updateCustomerFile(customer);
    updateBookFile(book);

    Rented rent;
    rent.R_ID = getNewRentId();
    rent.C_ID = C_ID;
    rent.B_ID = B_ID;
    time_t now = time(NULL);
    strftime(rent.rented_date, sizeof(rent.rented_date), "%d-%m-%Y", localtime(&now));
    rent.week = week;

    rented = fopen("rented.txt", "a");
    if (rented == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(rented, "%d %d %d %s %d\n", rent.R_ID, rent.C_ID, rent.B_ID, rent.rented_date, rent.week);
    fclose(rented);

    printf("Book rented successfully!\n");
    free(customer);
    free(book);
}

void DeliveryBook() { // This function handles the process of delivering a rented book.It takes the Rental ID as input from the user, updates the book's rental status, and removes the corresponding rental record from the rented.txt file.
    int R_ID;
    printf("Enter Rental ID: ");
    scanf("%d", &R_ID);

    FILE *file = fopen("rented.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (file == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Rented rent;
    int found = 0;
    while (fscanf(file, "%d %d %d %s %d", &rent.R_ID, &rent.C_ID, &rent.B_ID, rent.rented_date, &rent.week) != EOF) {
        if (rent.R_ID == R_ID) {
            Book* book = getBookById(rent.B_ID);
            if (book != NULL) {
                book->rented = 0;
                updateBookFile(book);
                free(book);
            }
            found = 1;
        } else {
            fprintf(temp, "%d %d %d %s %d\n", rent.R_ID, rent.C_ID, rent.B_ID, rent.rented_date, rent.week);
        }
    }
    fclose(file);
    fclose(temp);
    remove("rented.txt");
    rename("temp.txt", "rented.txt");

    if (found) {
        printf("Book delivered successfully!\n");
    } else {
        printf("Rental record not found!\n");
    }
}

void BurnBook() { // This function deletes the selected book from the library.
    int B_ID;
    printf("Enter Book ID to delete: ");
    scanf("%d", &B_ID);

    FILE *file = fopen("books.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (file == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Book book;
    int found = 0;
    while (fscanf(file, "%d %s %s %d %lf %d", &book.B_ID, book.name, book.author, &book.age_limit, &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID == B_ID) {
            found = 1;
        } else {
            fprintf(temp, "%d %s %s %d %lf %d\n", book.B_ID, book.name, book.author, book.age_limit, book.price_per_week, book.rented);
        }
    }
    fclose(file);
    fclose(temp);
    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found) {
        printf("Book deleted successfully!\n");
    } else {
        printf("Book not found!\n");
    }
}

void UpdateCustomer_Information() { // This function helps us the update the customers' information. User enters the ID then he/she change the information of the customer whixh has the selected ID.
    int C_ID;
    printf("Enter customer ID to update: ");
    scanf("%d", &C_ID);

    Customer* customer = getCustomerById(C_ID);
    if (customer == NULL) {
        printf("Customer not found!\n");
        return;
    }

    printf("Enter new name: ");
    scanf("%s", customer->name);
    printf("Enter new surname: ");
    scanf("%s", customer->surname);
    printf("Enter new age: ");
    scanf("%d", &customer->age);
    printf("Enter new wallet balance: ");
    scanf("%lf", &customer->wallet);

    updateCustomerFile(customer);
    printf("Customer information updated successfully!\n");
    free(customer);
}

void UpdateBook_Information() { // This function helps us the update the books' information. User enters the ID of the book then he/she change the informations of the book which has the selected ID.
    int B_ID;
    printf("Enter book ID to update: ");
    scanf("%d", &B_ID);

    Book* book = getBookById(B_ID);
    if (book == NULL) {
        printf("Book not found!\n");
        return;
    }

    printf("Enter new name: ");
    scanf("%s", book->name);
    printf("Enter new author: ");
    scanf("%s", book->author);
    printf("Enter new age limit: ");
    scanf("%d", &book->age_limit);
    printf("Enter new weekly price: ");
    scanf("%lf", &book->price_per_week);

    updateBookFile(book);
    printf("Book information updated successfully!\n");
    free(book);
}

void List_RentingCustomers() { // This function lists the customers whose renting the books and their informations. Renting, customer and the books ID, the date and the weeks they want to use the books.
    rented = fopen("rented.txt", "r");
    if (rented == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("Renting Customers:\n");
    Rented rent;
    while (fscanf(rented, "%d %d %d %s %d", &rent.R_ID, &rent.C_ID, &rent.B_ID, rent.rented_date, &rent.week) != EOF) {
        printf("Rental ID: %d, Customer ID: %d, Book ID: %d, Date: %s, Weeks: %d\n", rent.R_ID, rent.C_ID, rent.B_ID, rent.rented_date, rent.week);
    }
    fclose(rented);
}

void List_OfCustomers() { // This function lists the customers and their informations.
    customers = fopen("customers.txt", "r");
    if (customers == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("List of Customers:\n");
    Customer customer;
    while (fscanf(customers, "%d %s %s %d %lf", &customer.C_ID, customer.name, customer.surname, &customer.age, &customer.wallet) != EOF) {
        printf("ID: %d, Name: %s %s, Age: %d, Wallet: %.2lf\n", customer.C_ID, customer.name, customer.surname, customer.age, customer.wallet);
    }
    fclose(customers);
}

void List_OfBooks() { // This function lists the books in the library and their informatiıns like its name, the author's name, age limit of the book, price per week and it is rented or not.
    books = fopen("books.txt", "r");
    if (books == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("List of Books:\n");
    Book book;
    while (fscanf(books, "%d %s %s %d %lf %d", &book.B_ID, book.name, book.author, &book.age_limit, &book.price_per_week, &book.rented) != EOF) {
        printf("ID: %d, Name: %s, Author: %s, Age Limit: %d, Price per week: %.2lf, Rented: %d\n", book.B_ID, book.name, book.author, book.age_limit, book.price_per_week, book.rented);
    }
    fclose(books);
}

int main() {
    int choice;
    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Create Customer\n");
        printf("2. Deposit Money\n");
        printf("3. Add Book\n");
        printf("4. Rent Book\n");
        printf("5. Deliver Book\n");
        printf("6. Burn Book\n");
        printf("7. Update Customer Information\n");
        printf("8. Update Book Information\n");
        printf("9. List Renting Customers\n");
        printf("10. List of Customers\n");
        printf("11. List of Books\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                CreateCustomer();
                break;
            case 2:
                DepositMoney();
                break;
            case 3:
                AddBook();
                break;
            case 4:
                RentBook();
                break;
            case 5:
                DeliveryBook();
                break;
            case 6:
                BurnBook();
                break;
            case 7:
                UpdateCustomer_Information();
                break;
            case 8:
                UpdateBook_Information();
                break;
            case 9:
                List_RentingCustomers();
                break;
            case 10:
                List_OfCustomers();
                break;
            case 11:
                List_OfBooks();
                break;
            case 12:
                exit(0);
            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }
    }
    return 0;
}