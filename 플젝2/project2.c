#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ������ ������ ���� ����ü ����
typedef struct _parking_t {
    char name[100];
    char agency[100];
    char city[100];
    char district[100];
    double longitude;
    double latitude;
} parking_t;

// �迭����Ʈ ����ü ����
typedef struct _parking_list_t {
    int        len;
    parking_t* parkings;
} parking_list_t;

// utils_t ����ü ����
typedef struct _utils_t {
    void (*sort)   (parking_list_t*);
    void (*print)  (parking_list_t*);
    void (*filter)  (parking_list_t*);
} utils_t;

/* Array List */
int array_list_create(parking_list_t** list);
int array_list_destroy(parking_list_t** list);
int array_list_insert_last(parking_list_t* list, parking_t e);

/* Sort */
void selection_sort(parking_list_t* list);
void insertion_sort(parking_list_t* list);

void my_print(parking_list_t* list);
void my_sort(parking_list_t* list);
void my_filter(parking_list_t*list);




int main(int argc, char** argv) {
    // ���ڿ� ������ �迭
    char buffer[512] = "";

    // ����ü �迭 ����Ʈ ���� 
    parking_list_t* list=NULL;
    array_list_create(&list); 

    int op;
    char* fname = "free_parcking_spot-seoul.csv";
    FILE* fp;
    int i = 0; char* ptoken; char* delim = ",";
    //�Է¹��� ���ڿ��� ������ ������ ����ü ���� ����
    parking_t list_input; 
    
    utils_t my_util; // utils_t�� ����ü ���� my_util
    my_util.sort = my_sort; // sort�� my_sort �Լ� ����Ŵ
    my_util.print = my_print; // print�� my_print �Լ� ����Ŵ
    my_util.filter = my_filter; // filter�� my_filter �Լ� ����Ŵ

    // ���� �����Ͱ� NULL�̸� ���ڿ� ���
    if ((fp=fopen(fname,"r")) == NULL) {  
        printf("Cannot opne the file %s\n",fname);
        fclose(fp);
        return -1;
    }
    // ������ ù��° �� ����
    fgets(buffer, sizeof(buffer), fp); 
    // �ι�° ����� ����ü ������ ��� ���� 
    fgets(buffer, sizeof(buffer), fp); 

    while (1) {  
        ptoken = strtok(buffer, delim);
        strcpy(list_input.name, ptoken);

        ptoken = strtok(NULL, delim);
        strcpy(list_input.agency, ptoken);

        ptoken = strtok(NULL, delim);
        strcpy(list_input.city, ptoken);

        ptoken = strtok(NULL, delim);
        strcpy(list_input.district, ptoken);

        ptoken = strtok(NULL, delim);
        list_input.longitude = strtod(ptoken, NULL);   

        ptoken = strtok(NULL, delim);
        list_input.latitude = strtod(ptoken, NULL);

        // ��� �Ҵ��� �Ϸ��� ����ü�� ����ü �迭 ����Ʈ�� �߰� 
        array_list_insert_last(list, list_input); 
        // ���Ӱ� ���� ���ڿ��� NULL �� ��� while�� ���� 
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
    }
    printf("Stored all elements\n");
    fclose(fp); // �� �о�����Ƿ� ������ ����
   
    // �ɼ� 4�� �Էµ� �� ���� �ݺ�
    while (1) {   
        printf("====== Choose function ====== \n");
        printf("(1) sort \n");
        printf("(2) print \n");
        printf("(3) filter \n");
        printf("(4) exit \n");
        printf("Input option: ");
        scanf("%d", &op);
        if (op == 1)
            my_util.sort(list);
        else if (op == 2)
            my_util.print(list);
        else if (op == 3)
            my_util.filter(list);
        else if (op == 4) {
            printf("Exit the program");
            break; 
        }
        else
            printf("Invalid option\n");
    }
    // �迭 ����Ʈ �Ҹ��� ���� 
    array_list_destroy(&list); 

    return 0;
}

void my_print(parking_list_t* list) {
    int i;
    parking_t key;
    printf("====== Print all elements (len:%d)======\n", list->len);
    for (i = 0; i < list->len; i++) {
        key = list->parkings[i];
        printf("%s(%s) [%s %s (%.5lf, %.5lf)] \n", key.name, key.agency, key.city, key.district, key.longitude, key.latitude);
    }
}

void my_sort(parking_list_t* list) {
    int op;

    // �ɼ� �Է�
    printf("====== Choose a sorting algorithm ======\n");
    printf("(1) selection sort\n");
    printf("(2) insertion sort\n");
    printf("Input option 1:");
    scanf("%d", &op);

    if (op == 1) {
        // selection sort
        selection_sort(list);
    }
    else if (op == 2) {
        // insertion sort
        insertion_sort(list);
    }
    else {
        printf("Invalid option\n");
    }
}

void  my_filter(parking_list_t* list) {
   
    int op; int i;
    char city_name[100] = ""; 
    char district_name[100] = "";
    double min_long;
    double max_long;
    double min_lat;
    double max_lat;
    parking_t key;

    // ���ο� �迭 ����Ʈ ����
    parking_list_t* new_list = NULL;
    array_list_create(&new_list);

    // �ɼ� �Է�
    printf("====== Filter ====== \n");
    printf("(1) by city-district\n");
    printf("(2) by longitude-latitude\n");
    printf("Input option:");
    scanf("%d", &op);

    // �ñ��� ���� ���� 
    if (op == 1) {
        printf("Input city:");
        scanf("%s", city_name);
        printf("Input district:");
        scanf("%s", district_name);
        // ���θ��� ����Ʈ�� �Է¹��� ���� �����ϴ� ������ ���� 
        for (i = 0; i < list->len; i++) {
            key = list->parkings[i];
            if ((strcmp(key.city, city_name) == 0) && (strcmp(key.district, district_name)) == 0) {
                array_list_insert_last(new_list, key);
            }
        }
        // ���� ����Ʈ�� ù° �޸𸮺��� ���ο� ����Ʈ�� ����� �����͸� ����
        for (i = 0; i < new_list->len; i++) {
            key = new_list->parkings[i];
            list->parkings[i] = key;
        }
        // ����Ʈ�� ���̸� ���� ���� ����
        int list_len = list->len;
        int new_list_len = new_list->len;

        // ���� �迭 ����Ʈ�� ������ �޸𸮿������� ���ο� �����Ͱ� ��� �޸𸮰� ���ö����� ����
        for (i = 0; i < (list_len - new_list->len); i++) {
            if (list == NULL || list->len == 0)
                break;
            if (list->len == 1) {
                free(list->parkings);
                list->parkings = NULL;
                list->len--;
                break;
            }
            list->parkings = (parking_t*)realloc(list->parkings, sizeof(parking_t) * (list->len - 1));
            list->len--;
        }
        
        // ���ο� ����Ʈ �Ҹ� 
        array_list_destroy(&new_list);

    }
    // ���浵 ���� ���� 
    else if (op == 2) {
        printf("Input min_long:");
        scanf("%lf", &min_long);
        printf("Input max_long:");
        scanf("%lf", &max_long);
        printf("Input min_lat:");
        scanf("%lf", &min_lat);
        printf("Input max_lat:");
        scanf("%lf", &max_lat);
        // ���ο� ����Ʈ�� �Է¹��� ���� �����ϴ� ������ ����
        for (i = 0; i < list->len; i++) {
            key = list->parkings[i];
            if ((key.longitude >= min_long) && (key.longitude <= max_long) && (key.latitude >= min_lat) && (key.latitude <= max_lat)) {
                array_list_insert_last(new_list, key);
            }
        }
        // ���� ����Ʈ�� ù° �޸𸮺��� ���ο� ����Ʈ�� ����� �����͸� ���� 
        for (i = 0; i < new_list->len; i++) {
            key = new_list->parkings[i];
            list->parkings[i] = key;
        }
        // ���� �迭 ����Ʈ�� ���̸� ���� ���� ����
        int list_len = list->len;
        // ���� �迭 ����Ʈ�� ������ �޸𸮿������� ���ο� �����Ͱ� ��� �޸𸮰� ���ö����� ����
        for (i = 0; i < (list_len - new_list->len); i++) {
            if (list == NULL || list->len == 0)
                break;
            if (list->len == 1) {
                free(list->parkings);
                list->parkings = NULL;
                list->len--;
                break;
            }
            list->parkings = (parking_t*)realloc(list->parkings, sizeof(parking_t) * (list->len - 1));
            list->len--;
        }

        // ���ο� ����Ʈ �Ҹ� 
        array_list_destroy(&new_list);


    }
    // ���� ó�� 
    else{
        printf("Invalid option\n");
        array_list_destroy(&new_list);
    }
}

/* Array List */
int array_list_create(parking_list_t** list) {
    if (*list != NULL)
        return 0;
    *list = (parking_list_t*)malloc(sizeof(parking_list_t));
    (*list)->len = 0;
    (*list)->parkings = NULL;

    return 1;    // return Success
}
int array_list_destroy(parking_list_t** list) {
    if (*list == NULL)
        return 0;
    if ((*list)->parkings != NULL)
        free((*list)->parkings);
    free(*list);

    return 1;    // return Success
}
int array_list_insert_last(parking_list_t* list, parking_t e) {
    if (list == NULL)
        return 0;
    if (list->parkings == NULL)
        list->parkings = (parking_t*)malloc(sizeof(parking_t));
    else
        list->parkings = (parking_t*)realloc(list->parkings, sizeof(parking_t) * (list->len + 1));
    
    list->parkings[list->len] = e;
    list->len++;

    return 1;
}

/* Sort */
void selection_sort(parking_list_t* list) {
    int op;
    int i, j, min;
    parking_t temp;
    printf("====== Selection Sort ======\n");
    printf("(1) by city-district\n");
    printf("(2) by longitude-latitude\n");
    printf("Input option2:");
    scanf("%d", &op);

    if (op == 1) { 
        for (i = 0; i < list->len; i++) {
            min = i;
            for (j = i; j < list->len; j++) {
                if (strcmp(list->parkings[min].city, list->parkings[j].city) > 0) // �õ� �������� �������� ����
                    min = j;
                else if (strcmp(list->parkings[min].city, list->parkings[j].city) == 0) // �õ��� ���ٸ� ���� �������� �������� ����
                    if (strcmp(list->parkings[min].district, list->parkings[j].district) > 0)
                        min = j;
            }
            temp = list->parkings[i];
            list->parkings[i] = list->parkings[min];
            list->parkings[min] = temp;
        }
    }
    else if (op == 2) {
        for (i = 0; i < list->len; i++) {
            min = i;
            for (j = i; j < list->len; j++) {
                if (list->parkings[min].longitude > list->parkings[j].longitude) // �浵 �������� �������� ����
                    min = j;
                else if (list->parkings[min].longitude == list->parkings[j].longitude) // �浵�� ���ٸ� ���� �������� �������� ����
                    if (list->parkings[min].latitude > list->parkings[j].latitude)
                        min = j;
            }
            temp = list->parkings[i];                     // �ӽ� ����ü ������ ����ü �迭 ����Ʈ ��Ҹ� ���� �� ��ȯ
            list->parkings[i] = list->parkings[min];
            list->parkings[min] = temp;
        }
        
    }
    else {
        printf("Invalid option\n");
    }
}
void insertion_sort(parking_list_t* list) {
    int op;
    int i, j;
    parking_t temp;
    printf("====== Insertion Sort ======\n");
    printf("(1) by city-district\n");
    printf("(2) by longitude-latitude\n");
    printf("Input option2:");
    scanf("%d", &op);

    if (op == 1) {
        for (i = 1; i < list->len; i++) {
            temp = list->parkings[i];
            for (j = i; j > 0; j--) {
                if (strcmp(list->parkings[j - 1].city, temp.city) > 0) // �õ��� �������� �������� ����
                    list->parkings[j] = list->parkings[j - 1];
                else if (strcmp(list->parkings[j - 1].city, temp.city) == 0) { // �õ��� ���ٸ� ������ �������� �������� ����
                    if (strcmp(list->parkings[j - 1].district, temp.district) > 0)
                        list->parkings[j] = list->parkings[j - 1];
                    else
                        break;
                }
                else
                    break;
            }
            list->parkings[j] = temp;
        }
    }
    else if (op == 2) {
        for (i = 1; i < list->len; i++) {
            temp = list->parkings[i];
            for (j = i; j > 0; j--) {
                if (list->parkings[j - 1].longitude > temp.longitude) // �浵�� �������� �������� ����
                    list->parkings[j] = list->parkings[j - 1];
                else if (list->parkings[j - 1].longitude == temp.longitude) { // �浵�� ���ٸ� ������ �������� �������� ���� 
                    if (list->parkings[j - 1].latitude > temp.latitude)
                        list->parkings[j] = list->parkings[j - 1];
                    else
                        break;
                }
                else
                    break;
            }

            list->parkings[j] = temp;
        }
    }
    else {
        printf("Invalid option\n");
    }
}