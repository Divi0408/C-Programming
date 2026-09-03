#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 20
#define DAYS 7

typedef struct {
    char name[50], type[30];
    float limit, e[DAYS];
} Facility;

float total(float e[], int n) {
    if (n == 0)
        return 0;
    return e[n - 1] + total(e, n - 1);
}

int find(Facility f[], int n, char key[]) {
    for (int i = 0; i < n; i++)
        if (strstr(f[i].name, key))
            return i;
    return -1;
}

void input(Facility *f) {
    printf("Facility name: ");
    scanf(" %[^\n]", f->name);

    printf("Facility type: ");
    scanf(" %[^\n]", f->type);

    printf("Permitted weekly limit: ");
    scanf("%f", &f->limit);

    for (int i = 0; i < DAYS; i++) {
        printf("Day %d energy: ", i + 1);
        scanf("%f", &f->e[i]);
    }
}

void display(Facility *f) {
    float t = total(f->e, DAYS);

    printf("\n%s (%s)\n", f->name, f->type);
    printf("Weekly: %.2f kWh | Average: %.2f kWh\n",
           t, t / DAYS);

    printf("Daily: ");
    for (int i = 0; i < DAYS; i++)
        printf("%.1f ", f->e[i]);

    if (t > f->limit)
        printf("\nStatus: EXCEEDS LIMIT");
    else {
        int high = 0;
        for (int i = 1; i < DAYS; i++)
            if (f->e[i] > f->e[high])
                high = i;

        if (f->e[high] > 1.3 * t / DAYS)
            printf("\nStatus: ABNORMAL PEAK");
        else
            printf("\nStatus: NORMAL");
    }

    printf("\nPotential 10%% saving: %.2f kWh\n", t * 0.10);
}

void analyse(Facility f[], int n) {
    float day[DAYS] = {0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < DAYS; j++)
            day[j] += f[i].e[j];

    int high = 0, low = 0;

    for (int i = 1; i < DAYS; i++) {
        if (day[i] > day[high]) high = i;
        if (day[i] < day[low]) low = i;
    }

    printf("\nTotal weekly campus energy: %.2f kWh\n",
           total(day, DAYS));
    printf("Highest day: Day %d (%.2f kWh)\n",
           high + 1, day[high]);
    printf("Lowest day: Day %d (%.2f kWh)\n",
           low + 1, day[low]);

    for (int i = 0; i < n; i++) {
        float t = total(f[i].e, DAYS);
        float score = 100 - (t / f[i].limit) * 100;

        if (score < 0) score = 0;

        printf("%-20s Total: %7.2f  Score: %.1f\n",
               f[i].name, t, score);
    }
}

void sort(Facility f[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (total(f[j].e, DAYS) < total(f[j + 1].e, DAYS)) {
                Facility temp = f[j];
                f[j] = f[j + 1];
                f[j + 1] = temp;
            }

    printf("\nTop energy consumers:\n");
    for (int i = 0; i < n; i++)
        printf("%d. %s - %.2f kWh\n",
               i + 1, f[i].name, total(f[i].e, DAYS));
}

void save(Facility f[], int n) {
    FILE *fp = fopen("energy_records.txt", "w");

    if (!fp) {
        printf("File error.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s,%s,%.2f",
                f[i].name, f[i].type, f[i].limit);

        for (int j = 0; j < DAYS; j++)
            fprintf(fp, ",%.2f", f[i].e[j]);

        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Records saved successfully.\n");
}

void merge(Facility f[], int *n) {
    FILE *fp = fopen("merge_data.txt", "r");
    Facility x;
    int i, found;

    if (!fp) {
        printf("Merge file not found.\n");
        return;
    }

    while (fscanf(fp, " %49[^,],%29[^,],%f",
                  x.name, x.type, &x.limit) == 3) {

        for (i = 0; i < DAYS; i++)
            fscanf(fp, ",%f", &x.e[i]);

        found = find(f, *n, x.name);

        if (found >= 0)
            f[found] = x;
        else if (*n < MAX)
            f[(*n)++] = x;
    }

    fclose(fp);
    printf("Records merged successfully.\n");
}

int main() {
    Facility f[MAX];
    int n = 0, ch, i, k;
    char key[50];

    do {
        printf("\n\n===== SMART CAMPUS ENERGY SYSTEM =====\n");
        printf("1. Add Facility\n");
        printf("2. Display Facilities\n");
        printf("3. Analyse Energy\n");
        printf("4. Search Facility\n");
        printf("5. Sort by Consumption\n");
        printf("6. Merge Records\n");
        printf("7. Save Records\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {

        case 1:
            if (n < MAX) {
                input(&f[n]);
                n++;
            } else
                printf("Maximum facilities reached.\n");
            break;

        case 2:
            for (i = 0; i < n; i++)
                display(&f[i]);
            break;

        case 3:
            analyse(f, n);
            break;

        case 4:
            printf("Enter facility name: ");
            scanf(" %[^\n]", key);

            k = find(f, n, key);

            if (k >= 0)
                display(&f[k]);
            else
                printf("Facility not found.\n");
            break;

        case 5:
            sort(f, n);
            break;

        case 6:
            merge(f, &n);
            break;

        case 7:
            save(f, n);
            break;

        case 0:
            printf("Program ended.\n");
            break;

        default:
            printf("Invalid choice.\n");
        }

    } while (ch != 0);

    return 0;
}
