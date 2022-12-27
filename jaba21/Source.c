#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <malloc.h>
#include <string.h>

struct product {
	char productName[32];
	char createDay[24];
	char price[24];
};
typedef struct product Product;

struct productsConfig {
	Product* arr;
	int size;
};
typedef struct productsConfig ProductsConfig;

Product* inputProducts(Product* arr, int size);
int showProducts(Product* arr, int size);
int showOneProduct(Product product);
int compareCreateDay(const void* av, const void* bv);
int compareProductName(const void* av, const void* bv);
int compareCreateDayAndName(const void* av, const void* bv);
int loadProductsFromFile(ProductsConfig* config);
Product* changeRecord(Product* arr, int elementId);
int findRecords(ProductsConfig* config, int recordId, char recordValue[32]);
int sortRecords(ProductsConfig* config, int recordId, char recordValue[32]);
int writeToFile(ProductsConfig* config, char filename[24]);

int main() {
	setlocale(LC_ALL, "RUS");
	// 1
	int productsArraySize;
	int elementIdForChange;
	printf("Ведите количество продуктов: ");
	scanf("%d", &productsArraySize);
	Product* productsArray;
	productsArray = (Product*)malloc(productsArraySize * sizeof(Product));
	inputProducts(productsArray, productsArraySize);
	showProducts(productsArray, productsArraySize);
	showOneProduct(productsArray[1]);

	// 2
	qsort(productsArray, 4, sizeof(Product), compareCreateDay);
	showProducts(productsArray, productsArraySize);

	//3.1
	ProductsConfig config;
	config.arr = (Product*)malloc(1 * sizeof(Product));
	config.size = 3;
	ProductsConfig* productsConfig = &config;
	loadProductsFromFile(productsConfig);
	showProducts(productsConfig->arr, productsConfig->size);

	// 3.2
	printf("\n\nВведите элемент массива для изменения: ");
	scanf("%d", &elementIdForChange);
	changeRecord(productsConfig->arr, elementIdForChange);
	showProducts(productsConfig->arr, productsConfig->size);


	// 3.3
	int searchType;
	char searchValue[32];
	printf("\n\nВведите номер поля, по которому нужно искать элементы(1 - Название товара; 2-день создания; 3-цена):");
	scanf("%d", &searchType);
	printf("\nВведите значение, по которому нужно найти товары: ");
	scanf("%32s", searchValue);
	findRecords(productsConfig, searchType, searchValue);

	// 3.4
	sortRecords(productsConfig, searchType, searchValue);
	showProducts(productsConfig->arr, productsConfig->size);

	// 3.5
	char fileName[24];
	printf("\n\nВведите название файла для записи: ");
	scanf("%24s", fileName);
	writeToFile(productsConfig, fileName);
	return 0;
}

Product* inputProducts(Product* arr, int size) {
	for (int i = 0; i < size; i++) {
		printf("\n\nПродукт %d", i + 1);
		printf("\nВведите название продукта: ");
		scanf("%32s", &arr[i].productName);
		printf("\nВведите день создания продукта: ");
		scanf("%24s", &arr[i].createDay);
		printf("\nВведите цену продукта: ");
		scanf("%24s", &arr[i].price);
	}
	return arr;
}

int showProducts(Product* arr, int size) {
	for (int i = 0; i < size; i++) {
		printf("\n\nПродукт %d", i + 1);
		printf("\n		Название: %s", arr[i].productName);
		printf("\n		День создания: %s", arr[i].createDay);
		printf("\n		Цена: %s", arr[i].price);
	}
	return 0;
}

int showOneProduct(Product product) {
	printf("\n\nПродукт");
	printf("\n		Название: %s", product.productName);
	printf("\n		День создания: %s", product.createDay);
	printf("\n		Цена: %s", product.price);
	return 0;
}

int compareCreateDay(const void* av, const void* bv) {
	const Product* a = av, * b = bv;
	if (a->createDay < b->createDay) return -1;
	if (a->createDay > b->createDay) return 1;
	return 0;

}

int compareProductName(const void* av, const void* bv) {
	const Product* a = av, * b = bv;
	return -strcmp(a->productName, b->productName);
}

int compareCreateDayAndName(const void* av, const void* bv) {
	const Product* a = av, * b = bv;
	if (a->createDay < b->createDay) return -1;
	if (a->createDay > b->createDay) return 1;
	return -strcmp(a->productName, b->productName);
}

int loadProductsFromFile(ProductsConfig* config) {
	char fileName[32];
	char dataString[128];
	int counter = 0;
	int counterInputs = 0;

	FILE* file;
	printf("\n\nВведите название файла, в котором хранятся данные: ");
	scanf("%32s", fileName);
	file = fopen(fileName, "rt");
	while (!feof(file)) {
		counter++;
		config->arr = (Product*)realloc(config->arr, counter * sizeof(Product));
		fgets(dataString, sizeof(dataString), file);
		char* pch = strtok(dataString, " ");
		while (pch != NULL)
		{
			switch (counterInputs) {
			case 1:
				strcpy(config->arr[counter - 1].productName, pch);
				pch = strtok(NULL, " ");
				break;
			case 2:
				strcpy(config->arr[counter - 1].createDay, pch);
				pch = strtok(NULL, " ");
				break;
			case 3:
				strcpy(config->arr[counter - 1].price, pch);
				pch = strtok(NULL, " ");
				break;
			default:
				counterInputs = 0;
			}
			counterInputs++;
		}
	}
	config->size = counter;
	return 0;
}

Product* changeRecord(Product* arr, int elementId) {
	char name[32], createDay[24], price[24];
	printf("\n\nПродукт %d", elementId);
	printf("\n		Введите новое имя продукта: ");
	scanf("%32s", name);
	strcpy(arr[elementId - 1].productName, name);
	printf("\n		Введите новый день создания продукта: ");
	scanf("%24s", createDay);
	strcpy(arr[elementId - 1].createDay, createDay);
	printf("\n		Введите новую цену продукта: ");
	scanf("%24s", price);
	strcpy(arr[elementId - 1].price, price);
	return arr;
}

int findRecords(ProductsConfig* config, int recordId, char recordValue[32]) {
	for (int i = 0; i < config->size; i++) {
		switch (recordId) {
		case 1:
			if (strcmp(config->arr[i].productName, recordValue) == 0) {
				printf("\n\nПродукт %d", i + 1);
				printf("\n		Название: %s", config->arr[i].productName);
				printf("\n		День создания: %s", config->arr[i].createDay);
				printf("\n		Цена: %s", config->arr[i].price);
			}
		case 2:
			if (strcmp(config->arr[i].createDay, recordValue) == 0) {
				printf("\n\nПродукт %d", i + 1);
				printf("\n		Название: %s", config->arr[i].productName);
				printf("\n		День создания: %s", config->arr[i].createDay);
				printf("\n		Цена: %s", config->arr[i].price);
			}
		case 3:
			if (strcmp(config->arr[i].price, recordValue) == 0) {
				printf("\n\nПродукт %d", i + 1);
				printf("\n		Название: %s", config->arr[i].productName);
				printf("\n		День создания: %s", config->arr[i].createDay);
				printf("\n		Цена: %s", config->arr[i].price);
			}
		}
	}
	return 0;
}

int sortRecords(ProductsConfig* config, int recordId, char recordValue[32]) {
	Product* arr;
	arr = (Product*)malloc(config->size * sizeof(Product));
	int counter = 0;
	for (int i = 0; i < config->size; i++) {
		strcpy(arr[i].createDay, config->arr[i].createDay);
		strcpy(arr[i].productName, config->arr[i].productName);
		strcpy(arr[i].price, config->arr[i].price);
	}
	config->arr = (Product*)malloc(config->size * sizeof(Product));
	for (int i = 0; i < config->size; i++) {
		switch (recordId) {
		case 1:
			if (strcmp(arr[i].productName, recordValue) == 0) {
				strcpy(config->arr[counter].createDay, arr[i].createDay);
				strcpy(config->arr[counter].productName, arr[i].productName);
				strcpy(config->arr[counter].price, arr[i].price);
				counter++;
			}
			break;
		case 2:
			if (strcmp(arr[i].createDay, recordValue) == 0) {
				strcpy(config->arr[counter].createDay, arr[i].createDay);
				strcpy(config->arr[counter].productName, arr[i].productName);
				strcpy(config->arr[counter].price, arr[i].price);
				counter++;
			}
			break;
		case 3:
			if (strcmp(arr[i].price, recordValue) == 0) {
				strcpy(config->arr[counter].createDay, arr[i].createDay);
				strcpy(config->arr[counter].productName, arr[i].productName);
				strcpy(config->arr[counter].price, arr[i].price);
				counter++;
			}
			break;
		}
	}
	config->size = counter;
}

int writeToFile(ProductsConfig* config, char filename[24]) {
	FILE* out;
	if ((out = fopen(filename, "a")) == NULL) {
		printf("Ошибка открытия файла для записи");
		return 0;
	}
	for (int i = 0; i < config->size; i++) {
		fprintf(out, "%s %s %s", config->arr[i].productName, config->arr[i].createDay, config->arr[i].price);
	}
	return 0;
}