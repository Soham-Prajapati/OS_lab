#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct SearchArgs {
    const char *filename;
    const char *keyword;
    long start;
    long end;
    int count;
};

void *search_file(void *arg) {
    struct SearchArgs *args = (struct SearchArgs *)arg;
    FILE *file = fopen(args->filename, "r");
    if (!file) {
        perror("File open error");
        pthread_exit(0);
    }
    fseek(file, args->start, SEEK_SET);
    char line[1024];
    int local_count = 0;
    long pos = args->start;
    while (pos < args->end && fgets(line, sizeof(line), file)) {
        if (strstr(line, args->keyword)) local_count++;
        pos = ftell(file);
    }
    args->count = local_count;
    fclose(file);
    pthread_exit(0);
}

int main() {
    char filename[256], keyword[64];
    printf("Enter filename: ");
    scanf("%s", filename);
    printf("Enter keyword: ");
    scanf("%s", keyword);
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File open error");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fclose(file);
    long mid = filesize / 2;
    struct SearchArgs args1 = {filename, keyword, 0, mid, 0};
    struct SearchArgs args2 = {filename, keyword, mid, filesize, 0};
    pthread_t t1, t2;
    pthread_create(&t1, NULL, search_file, &args1);
    pthread_create(&t2, NULL, search_file, &args2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    int total = args1.count + args2.count;
    printf("Keyword '%s' found %d times in total.\n", keyword, total);
    printf("Thread 1 found %d, Thread 2 found %d.\n", args1.count, args2.count);
    return 0;
}
