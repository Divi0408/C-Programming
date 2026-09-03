#include <stdio.h>
#include <string.h>

#define MAX_FACILITIES 10
#define DAYS 7

typedef struct {
    char name[40];
    float limit;
    float energy[DAYS];
    float total;
    float average;
    float savings;
    float score;
} Facility;

void calculateAnalysis(Facility *f) {
    int i;
    f->total = 0;

    for (i = 0; i < DAYS; i++)
        f->total += f->energy[i];

    f->average = f->total / DAYS;
}

void calculateScore(Facility *f) {
    if (f->average <= f->limit * 0.75)
        f->score = 90;
    else if (f->average <= f->limit)
        f->score = 75;
    else if (f->average <= f->limit * 1.25)
        f->score = 55;
    else
        f->score = 35;
}

void displayAnalysis(Facility f) {
    int i, high = 0, low = 0;

    for (i = 1; i < DAYS; i++) {
        if (f.energy[i] > f.energy[high])
            high = i;
        if (f.energy[i] < f.energy[low])
            low = i;
    }

    printf("\nFacility: %s\n", f.name);
    printf("Total Consumption   : %.2f kWh\n", f.total);
    printf("Average Consumption : %.2f kWh\n", f.average);
    printf("Highest Day         : Day %d (%.2f kWh)\n",
           high + 1, f.energy[high]);
    printf("Lowest Day          : Day %d (%.2f kWh)\n",
           low + 1, f.energy[low]);

    if (f.average > f.limit)
        printf("Status              : EXCEEDS LIMIT\n");
    else
        printf("Status              : Within Limit\n");

    printf("Sustainability Score: %.0f/100\n", f.score);

    if (f.score >= 90)
        printf("Recommendation      : Maintain present energy-saving practices.\n");
    else if (f.score >= 75)
        printf("Recommendation      : Switch off unused lights and equipment.\n");
    else if (f.score >= 55)
        printf("Recommendation      : Reduce unnecessary equipment usage.\n");
    else
        printf("Recommendation      : Immediate energy audit and load reduction required.\n");
}

void sortFacilities(Facility f[], int n) {
    int i, j;
    Facility temp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (f[j].total < f[j + 1].total) {
                temp = f[j];
                f[j] = f[j + 1];
                f[j + 1] = temp;
            }
        }
    }
}

int searchFacility(Facility f[], int n, char key[]) {
    int i;

    for (i = 0; i < n; i++) {
        if (strcmp(f[i].name, key) == 0)
            return i;
    }

    return -1;
}

void saveReport(Facility f[], int n) {
    FILE *fp;
    int i;

    fp = fopen("energy_report.txt", "w");

    if (fp == NULL) {
        printf("Unable to create report file.\n");
        return;
    }

    fprintf(fp, "SMART CAMPUS ENERGY REPORT\n");
    fprintf(fp, "==========================\n");

    for (i = 0; i < n; i++) {
        fprintf(fp, "\nFacility: %s\n", f[i].name);
        fprintf(fp, "Total: %.2f kWh\n", f[i].total);
        fprintf(fp, "Average: %.2f kWh\n", f[i].average);
        fprintf(fp, "Limit: %.2f kWh\n", f[i].limit);
        fprintf(fp, "Score: %.0f/100\n", f[i].score);
    }

    fclose(fp);
    printf("\nReport saved successfully to energy_report.txt\n");
}

int main() {
    Facility f[MAX_FACILITIES];
    int n, i, j, choice, index;
    char key[40];
    float target;

    printf("SMART CAMPUS ENERGY MONITORING SYSTEM\n");
    printf("Enter number of facilities (1-%d): ", MAX_FACILITIES);
    scanf("%d", &n);

    if (n < 1 || n > MAX_FACILITIES) {
        printf("Invalid number of facilities.\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        printf("\nEnter facility name: ");
        scanf(" %[^\n]", f[i].name);

        printf("Enter permitted daily limit (kWh): ");
        scanf("%f", &f[i].limit);

        printf("Enter energy consumption for 7 days:\n");

        for (j = 0; j < DAYS; j++) {
            do {
                printf("Day %d: ", j + 1);
                scanf("%f", &f[i].energy[j]);

                if (f[i].energy[j] < 0)
                    printf("Energy cannot be negative. Enter again.\n");

            } while (f[i].energy[j] < 0);
        }

        calculateAnalysis(&f[i]);
        calculateScore(&f[i]);
    }

    do {
        printf("\n\n----- MENU -----\n");
        printf("1. Display Facility Analysis\n");
        printf("2. Sort by Highest Consumption\n");
        printf("3. Search Facility\n");
        printf("4. Calculate Potential Savings\n");
        printf("5. Save Sustainability Report\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                for (i = 0; i < n; i++)
                    displayAnalysis(f[i]);
                break;

            case 2:
                sortFacilities(f, n);

                printf("\nFacilities sorted by total consumption:\n");

                for (i = 0; i < n; i++)
                    printf("%d. %s - %.2f kWh\n",
                           i + 1, f[i].name, f[i].total);
                break;

            case 3:
                printf("Enter facility name to search: ");
                scanf(" %[^\n]", key);

                index = searchFacility(f, n, key);

                if (index != -1)
                    printf("Facility found: %s, Average = %.2f kWh\n",
                           f[index].name, f[index].average);
                else
                    printf("Facility not found.\n");
                break;

            case 4:
                printf("Enter reduction target percentage: ");
                scanf("%f", &target);

                if (target < 0 || target > 100) {
                    printf("Invalid target percentage.\n");
                } else {
                    for (i = 0; i < n; i++) {
                        f[i].savings = f[i].total * target / 100.0;

                        printf("%s: Potential saving = %.2f kWh\n",
                               f[i].name, f[i].savings);
                    }
                }
                break;

            case 5:
                saveReport(f, n);
                break;

            case 6:
                printf("Exiting program. Thank you.\n");
                break;

            default:
                printf("Invalid menu choice.\n");
        }

    } while (choice != 6);

    return 0;
}
