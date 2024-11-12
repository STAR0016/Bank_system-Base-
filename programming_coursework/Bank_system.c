#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>
#include <math.h>

void quit(){
    printf("Thank you for using our bank system, see you next time.");
    Sleep(2000);
}

void createAccount(){
      char name[20];
      char id[20];
      char Type[10];
      int accountNumber;
      int amount = 0;
      FILE *file;
      char  filename[25];

      //ask user for input to create account.
      printf("Enter your name: \n");
      scanf("%s",name);
      printf("Enter your ID number: \n");
      scanf("%s",id);
      printf("Enter your account type(current/ saving): \n");
      scanf("%s",Type);

      //produce a random account number
      srand((unsigned int)time(0));
      accountNumber = rand() % 990000 + 10000;

      //generate a file in database folder
      snprintf(filename,sizeof(filename),"database/%d.txt",accountNumber);
      file=fopen(filename, "w");

      //check if the file create succcess or not
      if(file == NULL){
        printf("Create file fail. \n");
        return; // stop the program to execute more instructions
      }

      //write information to this file
      fprintf(file, "Name: %s\n",name);
      fprintf(file, "ID: %s\n", id);
      fprintf(file, "Account Type: %s\n", Type);
      fprintf(file, "Account Number: %d\n", accountNumber);
      fprintf(file, "Amount: %d\n", amount);

      fclose(file);

      printf("Account created successfully!\n");
      printf("Account Number: %d\n", accountNumber);
      
}

void Display_account(){
   
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("database\\*.*", &findFileData); //search all files in database folder

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to open database folder.\n");
        return;
      }
     else{printf("List of account files:\n");
      }
    do {
        size_t length = strlen(findFileData.cFileName);
        if (length > 4) { 
            // Check that the last four characters of the file name are ".txt"
            if (strcmp(findFileData.cFileName + length - 4, ".txt") == 0) {
                printf("%s\n", findFileData.cFileName); 
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0); 

    FindClose(hFind);


    // The user enters the account number to view and prints out all the account information
    char accountFile[30];
    printf("\nEnter the account number you want to view: ");
    scanf("%s", accountFile);

    char filePath[50];
    snprintf(filePath, sizeof(filePath), "database\\%s.txt", accountFile);

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Account file not found.\n");
        return;
    }

    printf("Account Details:\n");
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
 
}

void delete_account(){
    char file_path[40];
    char acc_num[20];

    printf("Enter the account number to delete: \n");
    scanf("%s",acc_num);
    // use the input accountnumber to generate the filapath to delete
    snprintf(file_path,sizeof(file_path),"database/%s.txt",acc_num);
    
    if(remove(file_path)==0){
       printf("Account file'%s.txt' deleted successfully.\n",acc_num);

    }
    else{
      printf("Falied to delete the file, try again. \n");
    }
}

void Deposit(){
  //before deposit , tell the user the exist account in the database.
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("database\\*.*", &findFileData); //search all files in database folder

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to open database folder.\n");
        return;
      }
     else{printf("List of account files:\n");
      }
    do {
        size_t length = strlen(findFileData.cFileName);
        if (length > 4) { 
            // Check that the last four characters of the file name are ".txt"
            if (strcmp(findFileData.cFileName + length - 4, ".txt") == 0) {
                printf("%s\n", findFileData.cFileName); 
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0); 

    FindClose(hFind);

   // ask user which account number to deposit
    int accountNumber;
    float add_Amount;

    printf("Enter account number for deposit: ");
    scanf("%d", &accountNumber);

    char filename[30];
    snprintf(filename, sizeof(filename), "database/%d.txt", accountNumber);

    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Cant find this account, try again.\n");
        return;
    }

    printf("How much money to deposit(Accurate to two decimal places): \n");
    scanf("%f", &add_Amount);

    // Keep it to two decimal places
    add_Amount = floorf(add_Amount * 100) / 100;

    char line[20];
    float Now_Balance = 0;
    long balancePosition = -1;

    // Find and read the current balance
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line,"Amount:") != 0) {
            sscanf(line,"Amount:%f",&Now_Balance);
            balancePosition = ftell(file)- strlen(line)-1; 
            break;
        }
    }

    if (balancePosition == -1) {
        printf("Cant find the balance, try again.\n");
        fclose(file);
        return;
    }

    Now_Balance += add_Amount;

    // Go to the balance location and update a new balance
    fseek(file,balancePosition,SEEK_SET);
    fprintf(file,"Amount:%.2f\n",Now_Balance);

    fclose(file);
    printf("Deposited %.2f successfully. New balance is: %.2f\n",add_Amount,Now_Balance);
}

void Withdraw(){
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("database\\*.*", &findFileData); //search all files in database folder

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to open database folder.\n");
        return;
      }
     else{printf("List of account files:\n");
      }
    do {
        size_t length = strlen(findFileData.cFileName);
        if (length > 4) { 
            // Check that the last four characters of the file name are ".txt"
            if (strcmp(findFileData.cFileName + length - 4, ".txt") == 0) {
                printf("%s\n", findFileData.cFileName); 
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0); 

    FindClose(hFind);

    // Ask user which account number to withdraw
    int accountNumber;
    float withdraw_Amount=0;

    printf("\nEnter account number for withdrawal: ");
    scanf("%d", &accountNumber);

    char filename[30];
    snprintf(filename, sizeof(filename), "database/%d.txt", accountNumber);

    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Can't find this account, try again.\n");
        return;
    }

    char line[20];
    float Now_Balance = 0;
    long balancePosition = -1;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Amount:", 7) == 0) {
            sscanf(line, "Amount:%f", &Now_Balance);
            balancePosition = ftell(file) - (long int)strlen(line) - 1;
            break;
        }
    }

    printf("This account now have balance: %.2f\n",Now_Balance);
    printf("How much money to withdraw (Accurate to two decimal places): \n");
    scanf("%f", &withdraw_Amount);

    // Keep it to two decimal places
    withdraw_Amount = floorf(withdraw_Amount * 100) / 100;

    if (balancePosition == -1) {
        printf("Can't find the balance, try again.\n");
        fclose(file);
        return;
    }

    // Check if there is enough balance for the withdrawal
    if (Now_Balance < withdraw_Amount) {
        printf("Insufficient balance. Fail to withdraw\n");
        fclose(file);
        return;
    }

    Now_Balance -= withdraw_Amount;

    // Go to the balance location and update the new balance
    fseek(file, balancePosition, SEEK_SET);
    fprintf(file, "Amount:%.2f\n", Now_Balance);

    fclose(file);
    printf("Withdrew %.2f successfully. New balance is: %.2f\n", withdraw_Amount, Now_Balance);

}

void Remittance(){
    int from_acc, to_acc;
    float trans_money=0;
    float from_balance=0, to_balance=0;
    long from_position=-1, to_position=-1;
    char fromType[10],toType[10];
    float fee=0;

    //Display all accounts.
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("database\\*.*", &findFileData); //search all files in database folder

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to open database folder.\n");
        return;
      }
     else{printf("List of account files:\n");
      }
    do {
        size_t length = strlen(findFileData.cFileName);
        if (length > 4) { 
            // Check that the last four characters of the file name are ".txt"
            if (strcmp(findFileData.cFileName + length - 4, ".txt") == 0) {
                printf("%s\n", findFileData.cFileName); 
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0); 

    FindClose(hFind);

    //Ask user for essential input.
    printf("Enter the sender's account number: ");
    scanf("%d",&from_acc);
    printf("Enter the receiver's account number: ");
    scanf("%d", &to_acc);
    printf("Enter amount to transfer: ");
    scanf("%f", &trans_money);

    // Keep it to two decimal places
    trans_money = floorf(trans_money * 100) / 100;

    char fromfile[30],tofile[30];
    snprintf(fromfile,sizeof(fromfile),"database/%d.txt",from_acc);
    snprintf(tofile, sizeof(tofile), "database/%d.txt",to_acc);

    FILE *filefrom=fopen(fromfile, "r+");
    FILE *fileto=fopen(tofile,"r+");

    if (filefrom==NULL || fileto==NULL){
        printf("One of the accounts does not exist.\n");
        if (filefrom) fclose(filefrom);
        if (fileto) fclose(fileto);
        return;
    }

    char line[50];
    while (fgets(line, sizeof(line), filefrom) != NULL) {
        if (strncmp(line, "Account Type:", 13) == 0) {
            sscanf(line, "Account Type: %s", fromType);
        }
        if (strstr(line, "Amount:") != NULL) {
            sscanf(line, "Amount:%f", &from_balance);
            from_position = ftell(filefrom) - strlen(line) - 1;
            break;
        }
    }

    while (fgets(line, sizeof(line), fileto) != NULL) {
        if (strncmp(line, "Account Type:", 13) == 0) {
            sscanf(line, "Account Type: %s", toType);
        }
        if (strstr(line, "Amount:") != NULL) {
            sscanf(line, "Amount:%f", &to_balance);
            to_position = ftell(fileto) - strlen(line) - 1;
            break;
        }
    }

    //Error handling
     if (from_position == -1 || to_position == -1) {
        printf("Failed to get account's balance .\n");
        fclose(filefrom);
        fclose(fileto);
        return;
    }

    // Calculate remittance fee.
    if (strcmp(fromType, "saving") == 0 && strcmp(toType, "current") == 0) {
    fee = trans_money * 0.02f;
  } else if (strcmp(fromType, "current") == 0 && strcmp(toType, "saving") == 0) {
    fee = trans_money * 0.03f;
  }

  printf("\nSender's current balance: %.2f\n", from_balance);
  printf("Total amount: %.2f (including a remittance fee of %.2f)\n", trans_money + fee, fee);

  // Check if there is enough balance for the transaction
    if (from_balance < (trans_money + fee)) {
        printf("Insufficient balance in the sender's account.\n");
        fclose(filefrom);
        fclose(fileto);
        return;
    }

    // Update balances
    from_balance -= (trans_money + fee);
    to_balance += trans_money;

    // Write updated balances
    fseek(filefrom, from_position, SEEK_SET);
    fprintf(filefrom, "Amount:%.2f\n", from_balance);

    fseek(fileto, to_position, SEEK_SET);
    fprintf(fileto, "Amount:%.2f\n", to_balance);

    fclose(filefrom);
    fclose(fileto);

    printf("Transferred %.2f from account %d to account %d with a fee of %.2f. \nNew balance of sender: %.2f\n", 
           trans_money, from_acc, to_acc, fee, from_balance);

}

int main(){
    int choice=0;
    int run=1;

    // create database folder(if not exist alrady)
     struct stat st={0};
     if ( stat("database",&st)==-1){
      mkdir("database");
     }

    while(run){
        printf("\n====Bank_System ====\n");
        printf("1. Create New Bank Account\n");
        printf("2. Display the account's detail \n");
        printf("3. Delete Bank Account\n");
        printf("4. Deposit\n");
        printf("5. Withdrawal\n");
        printf("6. Remittance\n");
        printf("7. Quit\n");
        printf("Choose a function to continue: ");
        scanf("%d", &choice);


     switch (choice)
     {
      case 1:
        createAccount();
        break;

      case 2:
        Display_account();
        break;

      case 3:
        delete_account();
        break;

      case 4:
        Deposit();
        break;
      
      case 5:
        Withdraw();
        break;

      case 6:
        Remittance();
        break;

      case 7:
        run=0;//break the while loop and terminate this program.
        quit();
        break;
    
      default:
        printf("Invalid code, no function to be used. Type agian.\n");
        break;
     }
  }

    return 0;
}