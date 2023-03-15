#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"
#define MEMSTAT_FILE "memstat.txt"

#define EXCEPTION           "\033[1;31m%s\033[0m\n"
#define STATELESS           "\033[1;32m%s\033[0m\n"

#define OPEN_FILE_EXCEPTION         "+ OPEN_FILE_EXCEPTION +"

typedef struct Date {
    int day;
    int month;
    int year;
} date_struct;

typedef struct Node {
    char sender[50];
    char name[50];
    char worker[50];
    int weight;
    int count;
    date_struct date;
    struct Node *next;
} account;

void checkFileOpened(FILE *fin) {
    if (fin == NULL) {
        printf(EXCEPTION,
               OPEN_FILE_EXCEPTION
        );
        fprintf(fin,
                OPEN_FILE_EXCEPTION
        );
        exit(1);
    }
}

int compare(char *s, char *x) {
    int i = 0;
    for (i = 0; (s[i] != '\0' || x[i] != '\0'); i++)
        if (s[i] != x[i])
            return 0;
    return 1;
}

int compare_date(date_struct date1, date_struct date2) {
    if (date1.year == date2.year) {
        if (date1.month == date2.month) {
            if (date1.day == date2.day) {
                return 0;
            } else if (date1.day > date2.day) {
                return 1;
            } else {
                return -1;
            }
        } else if (date1.month > date2.month) {
            return 1;
        } else {
            return -1;
        }
    } else if (date1.year > date2.year) {
        return 1;
    } else {
        return -1;
    }
}

// Удаление записи с БД и освобождение памяти
void free_p(account *p) {
    int i = 0;
    p->date.year = -1;
    p->date.month = -1;
    p->date.day = -1;
    p->weight = -1;
    p->count = -1;
    for (i = 0; i < 50; i++) {
        p->sender[i] = '\0';
        p->name[i] = '\0';
        p->sender[i] = '\0';
    }
    p->next = NULL;
    free(p);
}

date_struct convert_date(char *values) {
    date_struct date;
    int y, m, d;
    sscanf(values, "%d.%d.%d", &y, &m, &d);
    date.year = y;
    date.month = m;
    date.day = d;
    return date;
}

// Error
void print_incorrect(FILE *fout, char line[]) {
    int i = 0;
    fprintf(fout, "incorrect: ");
    printf("incorrect: ");
    for (i = 0; i < 20; i++) {
        fprintf(fout, "%c", line[i]);
        printf("%c", line[i]);
    }
    fprintf(fout, "\n");
    printf("\n");
}

/* Проверяет, подходит ли запись */
int is_suitable(char fields[25][25], char conds[25][25], char values[25][40], account *p) {
    char sender_[] = "sender";
    char name_[] = "name";
    char weight_[] = "weight";
    char count_[] = "count";
    char worker_[] = "worker";
    char date_[] = "date";
    char equal_[] = "==";
    char moreequal_[] = ">=";
    char lessequal_[] = "<=";
    char more_[] = ">";
    char less_[] = "<";
    char notequal_[] = "!=";
    int j = 0, k = 0, i = 0, n = 0;
    int count1 = 0, count2 = 0;
    char *c;
    while (fields[n][0] != '\0') n++; //считаем количество условий
    while (j < n) {

        if (compare(fields[j], weight_)) // условие для веса
        {
            if (compare(conds[j], equal_)) //если в условии знак равно
            {
                if (p->weight == atoi(values[j])) k++;
            } else if (compare(conds[j], notequal_)) {
                if (p->weight != atoi(values[j])) k++;
            } else if (compare(conds[j], moreequal_)) {
                if (p->weight >= atoi(values[j])) k++;
            } else if (compare(conds[j], lessequal_)) {
                if (p->weight <= atoi(values[j])) k++;
            } else if (compare(conds[j], more_)) {
                if (p->weight > atoi(values[j])) k++;
            } else if (compare(conds[j], less_)) {
                if (p->weight < atoi(values[j])) k++;
            }
        } else if (compare(fields[j], count_)) //условие для количества
        {
            if (compare(conds[j], equal_)) {
                if (p->count == atoi(values[j])) k++;
            } else if (compare(conds[j], notequal_)) {
                if (p->count != atoi(values[j])) k++;
            } else if (compare(conds[j], moreequal_)) {
                if (p->count >= atoi(values[j])) k++;
            } else if (compare(conds[j], lessequal_)) {
                if (p->count <= atoi(values[j])) k++;
            } else if (compare(conds[j], more_)) {
                if (p->count > atoi(values[j])) k++;
            } else if (compare(conds[j], less_)) {
                if (p->count < atoi(values[j])) k++;
            }
        } else if (compare(fields[j], sender_)) {
            char ch_value[20];
            c = values[j];
            if (compare(conds[j], equal_)) {
                while (*c != '\0') {
                    i = 0;
                    while (i < 20) ch_value[i++] = '\0';
                    while (*c < 97 || *c > 122) c++;
                    i = 0;
                    while ((*c >= 97 && *c <= 122) || (*c == 95)) ch_value[i++] = *c++;
                    if (compare(p->sender, ch_value)) {
                        k++;
                        break;
                    }
                }
            } else if (compare(conds[j], notequal_)) {
                while (*c != '\0') {
                    i = 0;
                    while (i < 20) ch_value[i++] = '\0';
                    while ((*c < 97 || *c > 122) && (*c != '\0')) c++;
                    i = 0;
                    while ((*c >= 97 && *c <= 122) || (*c == 95)) ch_value[i++] = *c++;
                    count2++;
                    if (!(compare(p->sender, ch_value))) count1++;
                }
                if (count1 == count2) k++;
            }
        } else if (compare(fields[j], name_)) {
            char ch_value[20];
            c = values[j];
            if (compare(conds[j], equal_)) {
                while (*c != '\0') {
                    i = 0;
                    while (i < 20) ch_value[i++] = '\0';
                    while (*c < 97 || *c > 122) c++;
                    i = 0;
                    while ((*c >= 97 && *c <= 122) || (*c == 95)) ch_value[i++] = *c++;
                    if (compare(p->name, ch_value)) {
                        k++;
                        break;
                    }
                }
            } else if (compare(conds[j], notequal_)) {
                while (*c != '\0') {
                    i = 0;
                    while (i < 20) ch_value[i++] = '\0';
                    while ((*c < 97 || *c > 122) && (*c != '\0')) c++;
                    i = 0;
                    while ((*c >= 97 && *c <= 122) || (*c == 95)) ch_value[i++] = *c++;
                    count2++;
                    if (!(compare(p->name, ch_value))) count1++;
                }
                if (count1 == count2) k++;
            }
        } else if (compare(fields[j], worker_)) {
            char ch_value[20];
            c = values[j];
            if (compare(conds[j], equal_)) {
                while (*c != '\0') {
                    i = 0;
                    while (i < 20) ch_value[i++] = '\0';
                    while (*c < 97 || *c > 122) c++;
                    i = 0;
                    while ((*c >= 97 && *c <= 122) || (*c == 95)) ch_value[i++] = *c++;
                    if (compare(p->worker, ch_value)) {
                        k++;
                        break;
                    }
                }
            } else if (compare(conds[j], notequal_)) {
                while (*c != '\0') {
                    i = 0;
                    while (i < 20) ch_value[i++] = '\0';
                    while ((*c < 97 || *c > 122) && (*c != '\0')) c++;
                    i = 0;
                    while ((*c >= 97 && *c <= 122) || (*c == 95)) ch_value[i++] = *c++;
                    count2++;
                    if (!(compare(p->worker, ch_value))) count1++;
                }
                if (count1 == count2) k++;
            }
        } else if (compare(fields[j], date_)) {
            if (compare(conds[j], equal_)) {
                if (compare_date(p->date, convert_date(values[j])) == 0) k++;
            } else if (compare(conds[j], notequal_)) {
                if (compare_date(p->date, convert_date(values[j])) != 0) k++;
            } else if (compare(conds[j], more_)) {
                if (compare_date(p->date, convert_date(values[j])) == 1) k++;
            } else if (compare(conds[j], less_)) {
                if (compare_date(p->date, convert_date(values[j])) == -1) k++;
            } else if (compare(conds[j], moreequal_)) {
                if (compare_date(p->date, convert_date(values[j])) != -1) k++;
            } else if (compare(conds[j], lessequal_)) {
                if (compare_date(p->date, convert_date(values[j])) != 1) k++;
            }
        }
        j++;
    }
    if (n == k) return 1;
    else return 0;
}

// Процедура для вывода выбранных полей записи
void print_selected(account *p, char *order, FILE *fout) {
    char sender_[] = "sender";
    char name_[] = "name";
    char weight_[] = "weight";
    char count_[] = "count";
    char worker_[] = "worker";
    char date_[] = "date";
    char x[25];
    int i = 0, j = 0;
    while (order[j] != '\0') //пока не дойдём до конца порядка
    {
        i = 0;
        while (i < 25) x[i++] = '\0';
        i = 0;
        while ((order[j] >= 97 && order[j] <= 122) || order[j] == 95) x[i++] = order[j++];
        j++;
        if (compare(x, date_)) {
            fprintf(fout, "date=%d.%d.%d ", p->date.year, p->date.month, p->date.day);
            printf("date=%d.%d.%d ", p->date.year, p->date.month, p->date.day);
        } else if (compare(x, sender_)) //в зависимости от того, какое поле в массиве х, выводим в файл и консоль
        {
            fprintf(fout, "sender=%s ", p->sender);
            printf("sender=%s ", p->sender);
            continue;
        } else if (compare(x, name_)) {
            fprintf(fout, "name=%s ", p->name);
            printf("name=%s ", p->name);
            continue;
        } else if (compare(x, worker_)) {
            fprintf(fout, "worker=%s ", p->worker);
            printf("worker=%s ", p->worker);
            continue;
        } else if (compare(x, weight_)) {
            fprintf(fout, "weight=%d ", p->weight);
            printf("weight=%d ", p->weight);
            continue;
        } else if (compare(x, count_)) {
            fprintf(fout, "count=%d ", p->count);
            printf("count=%d ", p->count);
            continue;
        }
    }
    fprintf(fout, "\n");
    printf("\n");
}

// Процедура для вывода всех записей в БД с указанными полями
void select_all(account *head, char *order, FILE *fout) {
    int count = 0;
    account *p = head;
    while (p != NULL) {
        count++;
        print_selected(p, order, fout);
        p = p->next;
    }
    fprintf(fout, "select: %d\n", count);
    printf("select: %d\n", count);
}

// процедура для вывода записи, если он подходит под условия
void select1(char fields[25][25], char conds[25][25], char values[25][40], account *head, char *order, FILE *fout) {
    int count = 0;
    account *p = head;
    while (p != NULL) //проходим по всем записям
    {
        if (is_suitable(fields, conds, values, p)) //если подходит под условие
        {
            count++;
            print_selected(p, order, fout); //выводим выбранные поля этой записи
        }
        p = p->next; //проверяем следующую запись
    }
    fprintf(fout, "select: %d\n", count);
    printf("select: %d\n", count);
}

//функция для удаления нужных записей и получения нового указателя на начало списка
account *
delete1(char fields[25][25], char conds[25][25], char values[25][40], account *head, int *count_free, FILE *fout) {
    int count = 0;
    account *p = head, *p1 = p;
    while (p != NULL) //пока не дойдём до конца списка
    {
        if (is_suitable(fields, conds, values, p)) {
            if (p == head) //если запись является началом списка
            {
                head = p->next;
                p1 = head;
                free_p(p);
                (*count_free)++;
                p = p1;
            } else if (p->next == NULL) //если запись является концом спискка
            {
                p1 = head;
                while (p1->next != p) p1 = p1->next;
                p1->next = NULL;
                free_p(p);
                (*count_free)++;
                p = NULL;
            } else //запись является ни началом, ни концом списка
            {
                p1 = head;
                while (p1->next != p) p1 = p1->next;
                p1->next = p->next;
                free_p(p);
                (*count_free)++;
                p = p1->next;
            }
            count++;
        } else {
            p = p->next;
        }
    }
    fprintf(fout, "delete: %d\n", count);
    printf("delete: %d\n", count);
    return head;
}

// Изменяет данные полей
void update_p(char updates[1000], account *p) {
    char sender_[] = "sender";
    char name_[] = "name";
    char weight_[] = "weight";
    char count_[] = "count";
    char worker_[] = "worker";
    char date_[] = "date";
    int i = 0;
    char *c = updates;
    char field[25], value[20];
    for (i = 0; updates[i] != '\0'; i++); //считаем длину строки
    if (updates[i - 1] != ',') updates[i] = ','; //ставим в конец строки запятую, если её там нет
    while (*c != '\0') //пока не дойдём до конца строки
    {
        for (i = 0; i < 25; i++) field[i] = '\0';
        for (i = 0; i < 20; i++) value[i] = '\0';
        for (i = 0; *c != '='; i++) field[i] = *c++; //заполняем массив названием поля
        c++;
        for (i = 0; *c != ','; i++) value[i] = *c++;
        c++;
        if (compare(field, sender_)) strcpy(p->sender, value);
        else if (compare(field, name_)) strcpy(p->name, value);
        else if (compare(field, worker_)) strcpy(p->worker, value);
        else if (compare(field, weight_)) p->weight = atoi(value);
        else if (compare(field, count_)) p->count = atoi(value);
        else if (compare(field, date_)) p->date = convert_date(value);
    }
}

void update_list(char fields[25][25], char conds[25][25], char values[25][40], char updates[1000], account *head,
                 FILE *fout) {
    int count = 0;
    account *p = head;
    while (p != NULL) //пока не дойдёт до конца списка
    {
        if (is_suitable(fields, conds, values, p)) //проверяем запись на соответствие условиям
        {
            update_p(updates, p);
            count++;
        }
        p = p->next;
    }
    fprintf(fout, "update: %d\n", count);
    printf("update: %d\n", count);
}

void update_all_list(char updates[1000], account *head, FILE *fout) {
    int count = 0;
    account *p = head;
    while (p != NULL) {
        update_p(updates, p);
        count++;
        p = p->next;
    }
    fprintf(fout, "update: %d\n", count);
    printf("update: %d\n", count);
}

int is_similar(char fields[1000], account *p, account *p1) {
    char sender_[] = "sender";
    char name_[] = "name";
    char weight_[] = "weight";
    char count_[] = "count";
    char worker_[] = "worker";
    char date_[] = "date";
    char field[20] = {'\0'}, *c = fields;
    int i = 0;
    for (i = 0; fields[i] != '\0'; i++);
    fields[i] = ',';
    while (*c != '\0') {
        for (i = 0; *c != ','; i++)
            field[i] = *c++;
        c++;
        if (compare(field, sender_)) {
            if ((compare(p->sender, p1->sender)) == 0) return 0;
        } else {
            if (compare(field, name_)) {
                if ((compare(p->name, p1->name)) == 0) return 0;
            } else {
                if (compare(field, worker_)) {
                    if ((compare(p->worker, p1->worker)) == 0) return 0;
                } else {
                    if (compare(field, weight_)) {
                        if (p->weight != p1->weight) return 0;
                    } else {
                        if (compare(field, count_)) {
                            if (p->count != p1->count) return 0;
                        } else {
                            if (compare(field, date_)) {
                                if (!(compare_date(p->date, p1->date))) return 0;

                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

// Функция для удаления дубликатов
account *uniq(char fields[1000], account *head, int *count_free, FILE *fout) {
    int count = 0;
    account *p = head, *p1 = NULL, *prev = NULL, *p2 = NULL;
    while (p != NULL) {
        p1 = p->next;
        while (p1 != NULL) {
            if (is_similar(fields, p, p1)) {
                if (p == head) {
                    head = p->next;
                    free_p(p);
                    (*count_free)++;
                    count++;
                } else {
                    prev->next = p->next;
                    free_p(p);
                    (*count_free)++;
                    count++;
                }
                p = p1;
            }
            p1 = p1->next;
        }
        prev = p;
        p = p->next;
    }
    fprintf(fout, "uniq: %d\n", count);
    printf("uniq: %d\n", count);
    return head;
}

int main() {
    FILE *fin, *fout, *memstat;

    fin     = fopen(INPUT_FILE, "r");
    fout    = fopen(OUTPUT_FILE, "w");
    memstat = fopen(MEMSTAT_FILE, "w");
    checkFileOpened(fin);

    account *p = NULL, *cur = NULL, *head = NULL;
    int n = 0, countMalloc = 0, countRealloc = 0, countCalloc = 0, countFree = 0, i = 0, j = 0;
    char *p_cond = NULL, order_list[1000] = {'\0'}, first_cond[100] = {'\0'}, c = (char) NULL;
    char command[100]    = {'\0'},
         line[1000]      = {'\0'},
         strStatus[10]   = {'\0'},
         dates[15]       = {'\0'};
    char *p_sender = NULL, *p_name = NULL, *p_worker = NULL,
         *p_weight = NULL, *p_count = NULL, *p_date = NULL;
    char fields[25][25] = {'\0'}, conds[25][25] = {'\0'},
         values[25][40] = {'\0'}, updates[1000] = {'\0'};

    char exitCommand[]  = "exit";
    char insertQuery[]  = "insert";
    char deleteQuery[]  = "delete";
    char selectQuery[]  = "select";
    char updateQuery[]  = "update";
    char uniqQuery[]    = "uniq";
    char senderField[]  = "sender";
    char nameField[]    = "name";
    char weightField[]  = "weight";
    char countField[]   = "count";
    char workerField[]  = "worker";
    char dateField[]    = "date";

    // Reading records from a file
    while (1) {
        while (!feof(fin)) {
            for (i = 0; i < 100; i++) first_cond[i] = '\0';
            for (i = 0; i < 1000; i++) {
                line[i] = '\0';
                order_list[i] = '\0';
                updates[i] = '\0';
            }
            for (j = 0; j < 25; j++) {
                for (i = 0; i < 25; i++) {
                    fields[j][i] = '\0';
                    conds[j][i] = '\0';
                }
                for (i = 0; i < 40; i++) {
                    values[j][i] = '\0';
                }
            }
            i = 0; j = 0;
            fgets(line, 1000, fin);
            sscanf(line, "%s", command);
            if (*line == '\n') {
                continue;
            }
            if (compare(command, exitCommand)) {
                fprintf(memstat, "malloc:%d\nrealloc:%d\ncalloc:%d\nfree:%d\n", countMalloc, countRealloc,
                        countCalloc, countFree);
                return 0;
            }
            if (compare(command, insertQuery)) {
                if ((p_sender = strstr(line, "sender=")) == NULL) {
                    print_incorrect(fout, line);
                    continue;
                }
                if ((p_name = strstr(line, "name=")) == NULL) {
                    print_incorrect(fout, line);
                    continue;
                }
                if ((p_worker = strstr(line, "worker=")) == NULL) {
                    print_incorrect(fout, line);
                    continue;
                }
                if ((p_weight = strstr(line, "weight=")) == NULL) {
                    print_incorrect(fout, line);
                    continue;
                }
                if ((p_count = strstr(line, "count=")) == NULL) {
                    print_incorrect(fout, line);
                    continue;
                }
                if ((p_date = strstr(line, "date=")) == NULL) {
                    print_incorrect(fout, line);
                    continue;
                }
                cur = (account *) malloc(sizeof(account));
                countMalloc++;
                sscanf(p_date, "date=%s", dates);
                cur->date = convert_date(dates);
                sscanf(p_sender, "sender=%s", cur->sender);
                sscanf(p_name, "name=%s", cur->name);
                sscanf(p_worker, "worker=%s", cur->worker);
                sscanf(p_weight, "weight=%d", &cur->weight);
                sscanf(p_count, "count=%d", &cur->count);

                cur->next = NULL;
                if (head == NULL) {
                    head = cur;
                    fprintf(fout, "select: %d\n", ++n);
                    printf("select: %d\n", n);
                } else {
                    p = head; n = 1;
                    while (p->next != NULL) {
                        p = p->next;
                        n++;
                    }
                    p->next = cur;
                    fprintf(fout, "select: %d\n", ++n);
                    printf("select: %d\n", n);
                }
            } else if (compare(command, selectQuery)) {
                sscanf(line, "select %s %s", order_list, first_cond);
                if (*first_cond == '\0') {
                    select_all(head, order_list, fout);
                    continue;
                }
                p_cond = strstr(line, first_cond);
                while (*p_cond != '\0') {
                    i = 0;
                    while ((c = *p_cond++) != '<' && c != '>' && c != '/' && c != '=' && c != '!') {
                        fields[j][i++] = c;
                    }
                    p_cond--;
                    if (compare(fields[j], weightField) || compare(fields[j], countField)) {
                        i = 0;
                        while (((c = *p_cond++) < 48 || c > 57) && c != 45) {
                            conds[j][i++] = c;
                        }
                        p_cond--;
                        sscanf(p_cond, "%s", values[j]);
                        while (*p_cond != ' ' && *p_cond != '\0') p_cond++;
                        if (*p_cond == ' ') p_cond++;
                        j++;
                    } else if (compare(fields[j], senderField) || compare(fields[j], nameField) ||
                               compare(fields[j], workerField)) {
                        for (i = 0; *p_cond != '['; i++) {
                            conds[j][i] = *p_cond++;
                        }
                        p_cond++;
                        for (i = 0; *p_cond != ']'; i++) {
                            values[j][i] = *p_cond++;
                        }
                        if (values[j][0] == '\0' || conds[j][0] == '\0') {
                            print_incorrect(fout, line);
                            break;
                        }
                        while (*p_cond != ' ' && *p_cond != '\0') p_cond++;
                        if (*p_cond == ' ') p_cond++;
                        j++;
                    } else if (compare(fields[j], dateField)) {
                        sscanf(p_cond, "%s'%s'", conds[j], values[j]);
                        while (*p_cond != ' ' && *p_cond != '\0')
                            p_cond++; //двигаем указатель p_cond до пробела или конца строки
                        if (*p_cond == ' ') p_cond++;
                        j++; //считывание условия закончено
                    } else print_incorrect(fout, line);

                }
                select1(fields, conds, values, head, order_list, fout);
            } else if (compare(command, deleteQuery)) {
                sscanf(line, "delete %s", first_cond);
                p_cond = strstr(line, first_cond);
                if (*first_cond == '\0') {
                    print_incorrect(fout, line);
                    continue;
                }
                while (*p_cond != '\0') {
                    i = 0;
                    while ((c = *p_cond++) != '<' && c != '>' && c != '/' && c != '=' && c != '!') {
                        fields[j][i++] = c;
                    }
                    p_cond--;
                    if (compare(fields[j], weightField) || compare(fields[j], countField)) {
                        i = 0;
                        while (((c = *p_cond++) < 48 || c > 57) && c != 45) {
                            conds[j][i++] = c;
                        }
                        p_cond--;
                        sscanf(p_cond, "%s", values[j]);
                        while (*p_cond != ' ' && *p_cond != '\0') p_cond++;
                        if (*p_cond == ' ') p_cond++;
                        j++;
                    } else if (compare(fields[j], senderField) || compare(fields[j], nameField) ||
                               compare(fields[j], workerField)) {
                        for (i = 0; *p_cond != '['; i++) {
                            conds[j][i] = *p_cond++;
                        }
                        p_cond++;
                        for (i = 0; *p_cond != ']'; i++) {
                            values[j][i] = *p_cond++;
                        }
                        if (values[j][0] == '\0' || conds[j][0] == '\0') {
                            print_incorrect(fout, line);
                            break;
                        }
                        while (*p_cond != ' ' && *p_cond != '\0') p_cond++;
                        if (*p_cond == ' ') p_cond++;
                        j++;
                    } else if (compare(fields[j], dateField)) {
                        sscanf(p_cond, "%s'%s'", conds[j], values[j]);
                        while (*p_cond != ' ' && *p_cond != '\0')
                            p_cond++; //двигаем указатель p_cond до пробела или конца строки
                        if (*p_cond == ' ') p_cond++;
                        j++; //считывание условия закончено
                    } else print_incorrect(fout, line);
                }
                head = delete1(fields, conds, values, head, &countFree, fout);
            } else if (compare(command, updateQuery)) {
                sscanf(line, "update %s %s", updates, first_cond);
                p_cond = strstr(line, first_cond);
                if (*first_cond == '\0') {
                    update_all_list(updates, head, fout);
                    continue;
                }
                while (*p_cond != '\0') {
                    i = 0;
                    while ((c = *p_cond++) != '<' && c != '>' && c != '/' && c != '=' && c != '!') {
                        fields[j][i++] = c;
                    }
                    p_cond--;
                    if (compare(fields[j], weightField) || compare(fields[j], countField)) {
                        i = 0;
                        while (((c = *p_cond++) < 48 || c > 57) && c != 45) {
                            conds[j][i++] = c;
                        }
                        p_cond--;
                        sscanf(p_cond, "%s", values[j]);
                        while (*p_cond != ' ' && *p_cond != '\0') p_cond++;
                        if (*p_cond == ' ') p_cond++;
                        j++;
                    } else if (compare(fields[j], senderField) || compare(fields[j], nameField) ||
                               compare(fields[j], workerField)) {
                        for (i = 0; *p_cond != '['; i++) {
                            conds[j][i] = *p_cond++;
                        }
                        p_cond++;
                        for (i = 0; *p_cond != ']'; i++) {
                            values[j][i] = *p_cond++;
                        }
                        if (values[j][0] == '\0' || conds[j][0] == '\0') {
                            print_incorrect(fout, line);
                            break;
                        }
                        while (*p_cond != ' ' && *p_cond != '\0') p_cond++;
                        if (*p_cond == ' ') p_cond++;
                        j++;
                    } else if (compare(fields[j], dateField)) {
                        sscanf(p_cond, "%s'%s'", conds[j], values[j]);
                        while (*p_cond != ' ' && *p_cond != '\0')
                            p_cond++; //двигаем указатель p_cond до пробела или конца строки
                        if (*p_cond == ' ') p_cond++;
                        j++; //считывание условия закончено
                    } else print_incorrect(fout, line);
                }
                update_list(fields, conds, values, updates, head, fout);
            } else if (compare(command, uniqQuery)) {
                if (sscanf(line, "uniq %s", order_list) == EOF) {
                    print_incorrect(fout, line);
                    continue;
                }
                head = uniq(order_list, head, &countFree, fout);
            } else print_incorrect(fout, line);
        }
        fin = stdin;
    }
    return 0;
}
