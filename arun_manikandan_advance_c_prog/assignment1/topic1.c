// This is the solution for the TOPIC 1 Question of the assignment Submitted by Arun Manikandan

// TODO

// Below is the sample flow of the code:

//     1. Ask the user about what they want to do ie. Add task or Choose a day display taks or exit
//     2. Based on the user selection run down the respective function and perform action

//     Constraints:

//         - A day can have only a max of 3 tasks
//         - It must save the tasks respectively mapped to the specific day
//         - There must be options to add tasks per day and the users should be avble to choose the day

#include <stdio.h>
#include <string.h>

// Define a structure which is sippoosed to hold the data for a single day (Bottom-up)
// Using typedef to ensure that we dont have to mentio struct everytime we gonna use the structre
typedef struct {
    char dayName[10];
    char dayTasks[3][30];
    int currentTaskPerDay;
} Day;

// Defining an enum to map the specific days to an integer in the same order so we can use the same index later down for solving the Text-Integer Map problem
// Starts from 0 to N-1 where N is the total number of contents mentioned in the enum in this case from 0(Monday) to 7(InvalidDay)
typedef enum {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
    InvalidDay
} DayToIntMap;

// This function makes sure that the string input by the user is typecast to the DayToIntMap type so that the same can be used to retrive the index value
// WE use the basic strcmp to compare and return the appropriate values 
DayToIntMap getDayIndex(const char *dayStr) {
    if (strcmp(dayStr, "Monday") == 0) return Monday;
    if (strcmp(dayStr, "Tuesday") == 0) return Tuesday;
    if (strcmp(dayStr, "Wednesday") == 0) return Wednesday;
    if (strcmp(dayStr, "Thursday") == 0) return Thursday;
    if (strcmp(dayStr, "Friday") == 0) return Friday;
    if (strcmp(dayStr, "Saturday") == 0) return Saturday;
    if (strcmp(dayStr, "Sunday") == 0) return Sunday;
    return InvalidDay;
}

// Handler method to return the assigned task for the specific day in the week
// We pass the entire week structure(Array of Day struct) along with the index to specify the particular day we are trying to access
int getCurrentTaskCount(Day week[], DayToIntMap index) {
    return week[index].currentTaskPerDay;
}

// This get fucntion is used to return the remaining task count that the user can assing for the particular day
// We caluclate this using the MAX_COUNT - the number of task assigned for that particular day
int checkLeftTask(Day week[], DayToIntMap index) {
    return 3 - week[index].currentTaskPerDay;
}

// This SET function is used ot set the task for the particular day
// This also ensures that there are no 3 tasks already in the day to go by the constraint
void addTaskToDay(Day week[], DayToIntMap index, const char *task) {
    int current = week[index].currentTaskPerDay;
    if (current < 3) {
        strcpy(week[index].dayTasks[current], task);
        week[index].currentTaskPerDay++;
        printf("Task added to %s.\n", week[index].dayName);
    } else {
        printf("Task limit reached for %s!\n", week[index].dayName);
    }
}

// This get method ensures that the tasks assigned by the user to specific days are all mentioned at once
// We pass the array of Day struct and iterate throught the same and print the necessary details
void displayAllTasks(Day week[]) {
    printf("\n====== Weekly Task List ======\n");
    for (int i = 0; i < 7; i++) {
        printf("%s:\n", week[i].dayName);
        if (week[i].currentTaskPerDay == 0) {
            printf("  No tasks.\n");
        } else {
            for (int j = 0; j < week[i].currentTaskPerDay; j++) {
                printf("  - %s\n", week[i].dayTasks[j]);
            }
        }
    }
}

// Main loop for the program, entry point
int main() {
    
    // Defiing an array of struct type of Day representing a week and defining days
    Day week[7] = {
        {"Monday", {}, 0}, {"Tuesday", {}, 0}, {"Wednesday", {}, 0},
        {"Thursday", {}, 0}, {"Friday", {}, 0}, {"Saturday", {}, 0},
        {"Sunday", {}, 0},
    };

    // variables to hold the da yname and the task so that they can be used later 
    char day[20], task[30];
    int mainLoop = 1; //varaible to hold the flow of data this is what decides if the user wants to exit out or not

    while (mainLoop) {
        printf("\nEnter the day (or type 'exit' to quit): ");
        scanf("%s", day);

        if (strcmp(day, "exit") == 0) {
            mainLoop = 0;
            break;
        }

        DayToIntMap index = getDayIndex(day);
        if (index == InvalidDay) {
            printf("Invalid day entered. Try again.\n");
            continue;
        }

        int stayOnDay = 1;
        while (stayOnDay) {
            int left = checkLeftTask(week, index);
            printf("\n---- %s ----\n", week[index].dayName);
            printf("You can add %d more task(s).\n", left);
            printf("Choose an option:\n");
            printf("1. Add Task\n");
            printf("2. Select another day\n");
            printf("3. Show all tasks\n");
            printf("4. Exit\n");
            printf("Enter choice: ");

            int choice;
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    if (left == 0) {
                        printf("No more tasks can be added to %s.\n", week[index].dayName);
                    } else {
                        printf("Enter task: ");
                        scanf(" %[^\n]", task);  // Reads with spaces
                        addTaskToDay(week, index, task);
                    }
                    break;
                case 2:
                    stayOnDay = 0;
                    break;
                case 3:
                    displayAllTasks(week);
                    break;
                case 4:
                    stayOnDay = 0;
                    mainLoop = 0;
                    break;
                default:
                    printf("Invalid choice.\n");
            }
        }
    }

    printf("\nFinal Task Overview:\n");
    displayAllTasks(week);

    return 0;
}
