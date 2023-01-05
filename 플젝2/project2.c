#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 주차장 정보를 담을 구조체 구현
typedef struct _parking_t {
    char name[100];
    char agency[100];
    char city[100];
    char district[100];
    double longitude;
    double latitude;
} parking_t;

// 배열리스트 구조체 구현
typedef struct _parking_list_t {
    int        len;
    parking_t* parkings;
} parking_list_t;

// utils_t 구조체 구현
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
    // 문자열 저장할 배열
    char buffer[512] = "";

    // 구조체 배열 리스트 생성 
    parking_list_t* list=NULL;
    array_list_create(&list); 

    int op;
    char* fname = "free_parcking_spot-seoul.csv";
    FILE* fp;
    int i = 0; char* ptoken; char* delim = ",";
    //입력받은 문자열을 나눠서 저장할 구조체 변수 선언
    parking_t list_input; 
    
    utils_t my_util; // utils_t의 구조체 변수 my_util
    my_util.sort = my_sort; // sort가 my_sort 함수 가리킴
    my_util.print = my_print; // print가 my_print 함수 가리킴
    my_util.filter = my_filter; // filter가 my_filter 함수 가리킴

    // 파일 포인터가 NULL이면 문자열 출력
    if ((fp=fopen(fname,"r")) == NULL) {  
        printf("Cannot opne the file %s\n",fname);
        fclose(fp);
        return -1;
    }
    // 파일의 첫번째 행 읽음
    fgets(buffer, sizeof(buffer), fp); 
    // 두번째 행부터 구조체 변수에 담기 시작 
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

        // 멤버 할당을 완료한 구조체를 구조체 배열 리스트에 추가 
        array_list_insert_last(list, list_input); 
        // 새롭게 읽은 문자열이 NULL 일 경우 while문 종료 
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
            break;
    }
    printf("Stored all elements\n");
    fclose(fp); // 다 읽어왔으므로 파일을 닫음
   
    // 옵션 4가 입력될 때 까지 반복
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
    // 배열 리스트 소멸후 종료 
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

    // 옵션 입력
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

    // 새로운 배열 리스트 생성
    parking_list_t* new_list = NULL;
    array_list_create(&new_list);

    // 옵션 입력
    printf("====== Filter ====== \n");
    printf("(1) by city-district\n");
    printf("(2) by longitude-latitude\n");
    printf("Input option:");
    scanf("%d", &op);

    // 시군구 기준 필터 
    if (op == 1) {
        printf("Input city:");
        scanf("%s", city_name);
        printf("Input district:");
        scanf("%s", district_name);
        // 새로만든 리스트에 입력받은 조건 만족하는 데이터 저장 
        for (i = 0; i < list->len; i++) {
            key = list->parkings[i];
            if ((strcmp(key.city, city_name) == 0) && (strcmp(key.district, district_name)) == 0) {
                array_list_insert_last(new_list, key);
            }
        }
        // 기존 리스트의 첫째 메모리부터 새로운 리스트에 저장된 데이터를 복사
        for (i = 0; i < new_list->len; i++) {
            key = new_list->parkings[i];
            list->parkings[i] = key;
        }
        // 리스트의 길이를 담은 변수 선언
        int list_len = list->len;
        int new_list_len = new_list->len;

        // 기존 배열 리스트의 마지막 메모리에서부터 새로운 데이터가 담긴 메모리가 나올때까지 삭제
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
        
        // 새로운 리스트 소멸 
        array_list_destroy(&new_list);

    }
    // 위경도 범위 필터 
    else if (op == 2) {
        printf("Input min_long:");
        scanf("%lf", &min_long);
        printf("Input max_long:");
        scanf("%lf", &max_long);
        printf("Input min_lat:");
        scanf("%lf", &min_lat);
        printf("Input max_lat:");
        scanf("%lf", &max_lat);
        // 새로운 리스트에 입력받은 조건 만족하는 데이터 저장
        for (i = 0; i < list->len; i++) {
            key = list->parkings[i];
            if ((key.longitude >= min_long) && (key.longitude <= max_long) && (key.latitude >= min_lat) && (key.latitude <= max_lat)) {
                array_list_insert_last(new_list, key);
            }
        }
        // 기존 리스트의 첫째 메모리부터 새로운 리스트에 저장된 데이터를 저장 
        for (i = 0; i < new_list->len; i++) {
            key = new_list->parkings[i];
            list->parkings[i] = key;
        }
        // 기존 배열 리스트의 길이를 담을 변수 선언
        int list_len = list->len;
        // 기존 배열 리스트의 마지막 메모리에서부터 새로운 데이터가 담긴 메모리가 나올때까지 삭제
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

        // 새로운 리스트 소멸 
        array_list_destroy(&new_list);


    }
    // 예외 처리 
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
                if (strcmp(list->parkings[min].city, list->parkings[j].city) > 0) // 시도 기준으로 오름차순 정렬
                    min = j;
                else if (strcmp(list->parkings[min].city, list->parkings[j].city) == 0) // 시도가 같다면 군구 기준으로 오름차순 정렬
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
                if (list->parkings[min].longitude > list->parkings[j].longitude) // 경도 기준으로 오름차순 정렬
                    min = j;
                else if (list->parkings[min].longitude == list->parkings[j].longitude) // 경도가 같다면 위도 기준으로 오름차순 정렬
                    if (list->parkings[min].latitude > list->parkings[j].latitude)
                        min = j;
            }
            temp = list->parkings[i];                     // 임시 구조체 변수에 구조체 배열 리스트 요소를 담은 후 교환
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
                if (strcmp(list->parkings[j - 1].city, temp.city) > 0) // 시도를 기준으로 오름차순 정렬
                    list->parkings[j] = list->parkings[j - 1];
                else if (strcmp(list->parkings[j - 1].city, temp.city) == 0) { // 시도가 같다면 군구를 기준으로 오름차순 정렬
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
                if (list->parkings[j - 1].longitude > temp.longitude) // 경도를 기준으로 오름차순 정렬
                    list->parkings[j] = list->parkings[j - 1];
                else if (list->parkings[j - 1].longitude == temp.longitude) { // 경도가 같다면 위도를 기준으로 오름차순 정렬 
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