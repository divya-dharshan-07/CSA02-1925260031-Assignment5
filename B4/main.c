#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 2
#define LINE_CAP 32

typedef struct {
    int id;
    char name[30];
    char blood_group[5];
    int age;
} Donor;

typedef struct {
    Donor *data;
    size_t size;
    size_t capacity;
} DonorList;

static int add_donor(DonorList *list, Donor donor, int announce)
{
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity * 2;

        if (new_capacity < list->capacity ||
            new_capacity > SIZE_MAX / sizeof(Donor)) {
            return 0;
        }

        Donor *temporary = realloc(list->data,
                                   new_capacity * sizeof(Donor));
        if (temporary == NULL) {
            return 0;
        }

        list->data = temporary;
        list->capacity = new_capacity;
    }

    list->data[list->size++] = donor;

    printf("added %d (%s): size = %zu, capacity = %zu\n",
           donor.id, donor.blood_group, list->size, list->capacity);
    return 1;
}

static int create_input_file(void)
{
    FILE *fp = fopen("donors_input.txt", "w");
    if (fp == NULL) {
        perror("donors_input.txt");
        return 0;
    }

    if (fprintf(fp, "7001 Divya O+ 20\n"
                    "7002 Arun A+ 25\n"
                    "7003 Kiran B- 30\n"
                    "7004 Ravi AB+ 22\n"
                    "7005 Priya O- 28\n") < 0) {
        perror("write donors_input.txt");
        fclose(fp);
        return 0;
    }

    if (fclose(fp) != 0) {
        perror("close donors_input.txt");
        return 0;
    }
    return 1;
}

static int load_donors(DonorList *list)
{
    FILE *fp = fopen("donors_input.txt", "r");
    if (fp == NULL) {
        perror("donors_input.txt");
        return 0;
    }

    Donor donor;
    int result;

    while ((result = fscanf(fp, "%d %29s %4s %d",
                            &donor.id, donor.name,
                            donor.blood_group, &donor.age)) == 4) {
        if (!add_donor(list, donor, 0)) {
            fprintf(stderr, "Unable to grow donor list.\n");
            fclose(fp);
            return 0;
        }
    }

    if (!feof(fp)) {
        fprintf(stderr, "Invalid donor record in donors_input.txt.\n");
        fclose(fp);
        return 0;
    }

    if (fclose(fp) != 0) {
        perror("close donors_input.txt");
        return 0;
    }
    return 1;
}

static int generate_report(const DonorList *list)
{
    FILE *fp = fopen("eligibility_report.txt", "w");
    if (fp == NULL) {
        perror("eligibility_report.txt");
        return 0;
    }

    puts("\nNightly donor eligibility report:");
    fprintf(fp, "Nightly donor eligibility report:\n");

    for (size_t i = 0; i < list->size; ++i) {
        const Donor *donor = &list->data[i];
        const char *status =
            (donor->age >= 18 && donor->age <= 65)
                ? "Eligible" : "Not eligible";

        printf("%d %s %s: %s\n",
               donor->id, donor->name, donor->blood_group, status);
        fprintf(fp, "%d %s %s: %s\n",
                donor->id, donor->name, donor->blood_group, status);
    }

    if (fclose(fp) != 0) {
        perror("close eligibility_report.txt");
        return 0;
    }
    return 1;
}

int main(void)
{
    DonorList list = {NULL, 0, INITIAL_CAPACITY};
    list.data = malloc(list.capacity * sizeof(Donor));

    if (list.data == NULL) {
        perror("malloc");
        return 1;
    }

    const Donor samples[] = {
        {7001, "Divya", "O+", 20},
        {7002, "Arun", "A+", 25},
        {7003, "Kiran", "B-", 30},
        {7004, "Ravi", "AB+", 22},
        {7005, "Priya", "O-", 28}
    };
    const size_t sample_count = sizeof(samples) / sizeof(samples[0]);

    for (size_t i = 0; i < sample_count; ++i) {
        if (!add_donor(&list, samples[i], 1)) {
            fprintf(stderr, "Unable to add sample donor.\n");
            free(list.data);
            return 1;
        }
    }

    printf("allocated bytes=%zu\n", list.capacity * sizeof(Donor));

    /* Reuse the list for records loaded from the persistent input file. */
    list.size = 0;

    if (!create_input_file() || !load_donors(&list) ||
        !generate_report(&list)) {
        fprintf(stderr, "B4 file/report operation failed.\n");
        free(list.data);
        return 1;
    }

    free(list.data);
    list.data = NULL;
    list.size = 0;
    list.capacity = 0;
    return 0;
}
