#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define color SetConsoleTextAttribute

// Estructura para almacenar datos de cualquier tipo
struct Nodo {
    void* datos;
    struct Nodo* siguiente;
};

// Diseño
void borrar();
char menu();
void gotoxy(int x, int y);
int margen(int a, int b, int c, int d);

// Funciones
void alta(struct Nodo** lista, size_t tamanoDatos);
void baja(struct Nodo** lista, size_t tamanoDatos);
void consulta(struct Nodo* lista, size_t tamanoDatos, void (*imprimirDatos)(void*));
void modificar(struct Nodo* lista, size_t tamanoDatos);
void ordenar(struct Nodo** lista, size_t tamanoDatos, int (*compararDatos)(const void*, const void*));
void guardar(struct Nodo* lista, size_t tamanoDatos, const char* nombreArchivo);
void abrir(struct Nodo** lista, size_t tamanoDatos, const char* nombreArchivo);
void mostrarLista(struct Nodo* lista, size_t tamanoDatos, void (*imprimirDatos)(void*));
void liberarMemoria(struct Nodo* lista, void (*liberarDatos)(void*));
void mostrarMatriculas(struct Nodo* lista);
void imprimirDatosAlumno(void* datos);
int compararAlumnos(const void* a, const void* b);
void ordenar(struct Nodo** lista, size_t tamanoDatos, int (*compararDatos)(const void*, const void*));
void swap(struct Nodo* a, struct Nodo* b);
// Estructura para almacenar los datos de un alumno
struct Alumno {
    int matricula;
    char nombre[50];
    float promedio;
    int semestre;
    char carrera[50]; 
};

int main() {
    struct Nodo* lista = NULL;
    char opcion;

    do {
        opcion = menu();
        system("cls");

        switch (opcion) {
            case 'A':
                alta(&lista, sizeof(struct Alumno));
                break;
            case 'B':
                baja(&lista, sizeof(struct Alumno));
                break;
            case 'C':
                consulta(lista, sizeof(struct Alumno), imprimirDatosAlumno);
                break;
            case 'M':
                modificar(lista, sizeof(struct Alumno));
                break;
            case 'O':
                ordenar(&lista, sizeof(struct Alumno), compararAlumnos);
                break;
            case 'G':
                guardar(lista, sizeof(struct Alumno), "alumnos.dat");
                break;
            case 'L':
                abrir(&lista, sizeof(struct Alumno), "alumnos.dat");
                break;
            case 'S':
                margen(1, 1, 100, 25);
                gotoxy(40, 13);
                printf("Saliendo del programa...");
                Sleep(1000);
                system("cls");
                break;
            default:
                gotoxy(35, 2);
                printf("Opcion no valida. Intente de nuevo.");
        }
        mostrarMatriculas(lista);
    } while (opcion != 'S');
    return 0;
}
//Compara datos de los alumnos (se usa para ordenamiento)
int compararAlumnos(const void* a, const void* b) {
    return ((struct Alumno*)a)->matricula - ((struct Alumno*)b)->matricula;
}

void alta(struct Nodo** lista, size_t tamanoDatos) {
    struct Alumno* nuevoAlumno = (struct Alumno*)malloc(sizeof(struct Alumno));
    if (!nuevoAlumno) {
        gotoxy(30, 13);
        printf("Error al asignar memoria para el nuevo alumno.\n");
        borrar();
        return;
    }

    margen(1, 1, 100, 25);
    margen(40, 2, 60, 4);
    gotoxy(48, 3);
    printf("ALTA");

    gotoxy(10, 8);
    printf("Matricula del alumno: ");
    scanf("%d", &(nuevoAlumno->matricula));

    // Verificar si la matricula ya existe en la lista
    struct Nodo* actual = *lista;
    while (actual != NULL) {
        if (((struct Alumno*)actual->datos)->matricula == nuevoAlumno->matricula) {
            free(nuevoAlumno);
            gotoxy(30, 20);
            printf("Ya existe un alumno con la matricula ingresada");
            borrar();
            return;
        }
        actual = actual->siguiente;
    }

    gotoxy(10, 10);
    printf("Nombre del alumno: ");
    //aceptar espacios
    getchar();
    fgets(nuevoAlumno->nombre, sizeof(nuevoAlumno->nombre), stdin);
    nuevoAlumno->nombre[strcspn(nuevoAlumno->nombre, "\n")] = '\0';

    gotoxy(10, 12);
    printf("Promedio del alumno: ");
    scanf("%f", &(nuevoAlumno->promedio));

    gotoxy(10, 14);
    printf("Semestre del alumno: ");
    scanf("%d", &(nuevoAlumno->semestre));

    gotoxy(10, 16);
    printf("Carrera del alumno: ");
    getchar();
    //espacios
    fgets(nuevoAlumno->carrera, sizeof(nuevoAlumno->carrera), stdin);
    nuevoAlumno->carrera[strcspn(nuevoAlumno->carrera, "\n")] = '\0';

    Sleep(100);

    struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
    if (!nuevoNodo) {
        free(nuevoAlumno);
        gotoxy(30, 20);
        printf("Error al asignar memoria para el nuevo nodo.\n");
        return;
    }

    nuevoNodo->datos = nuevoAlumno;
    nuevoNodo->siguiente = *lista;
    *lista = nuevoNodo;

    gotoxy(30, 20);
    printf("Alumno dado de alta correctamente.");
    borrar();
}

void baja(struct Nodo** lista, size_t tamanoDatos) {
	margen(1, 1, 100, 25);
	margen(40, 2, 60,4);
    gotoxy(48, 3);
	printf("BAJA");
	
    if (*lista == NULL) {
    	gotoxy(35, 13);
        printf("La lista de alumnos esta vacia.\n");
        borrar();
        return;
    }

    int matriculaBusqueda;
    gotoxy(10, 6);
    printf("Ingrese la matricula del alumno que desea dar de baja: ");
    scanf("%d", &matriculaBusqueda);

    struct Nodo* actual = *lista;
    struct Nodo* anterior = NULL;

    while (actual != NULL && ((struct Alumno*)actual->datos)->matricula != matriculaBusqueda) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
    	gotoxy(10, 15);
        printf("No se encontraron datos pertenecientes a la matricula %d.\n", matriculaBusqueda);
        borrar();
        return;
    }
	
	gotoxy(10, 8);
    printf("Datos encontrados:\n");
    imprimirDatosAlumno(actual->datos);

    char respuesta;
    gotoxy(10, 17);
    printf("Desea dar de baja al alumno? (S/N): ");
    scanf(" %c", &respuesta);

    if (respuesta == 'S' || respuesta == 's') {
        if (anterior == NULL) {
            *lista = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }
        free(actual->datos);
        free(actual);
        gotoxy(30, 20);
        printf("Alumno dado de baja correctamente.\n");
    } else {
    	gotoxy(30, 20);
        printf("Operacion de baja cancelada.\n");
    }
    borrar();
}

void consulta(struct Nodo* lista, size_t tamanoDatos, void (*imprimirDatos)(void*)) {
	margen(1, 1, 100, 25);
	margen(40, 2, 60,4);
    gotoxy(45, 3);
	printf("CONSULTA");
	
    if (lista == NULL) {
    	gotoxy(35, 13);
        printf("La lista de alumnos esta vacia.\n");
        borrar();
        return;
    }

    int matriculaBusqueda;
    gotoxy(10, 6);
    printf("Ingrese la matricula del alumno que desea buscar: ");
    scanf("%d", &matriculaBusqueda);

    struct Nodo* actual = lista;

    while (actual != NULL && ((struct Alumno*)actual->datos)->matricula != matriculaBusqueda) {
        actual = actual->siguiente;
    }

    if (actual == NULL) {
    	gotoxy(10, 8);
        printf("No se encontraron datos pertenecientes a la matricula %d.\n", matriculaBusqueda);
    } else {
    	gotoxy(10, 8);
        printf("Datos encontrados:\n");
        imprimirDatos(actual->datos);
    }
    borrar();
}

void modificar(struct Nodo* lista, size_t tamanoDatos) {
		HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    margen(1, 1, 100, 25);
    margen(40, 2, 60, 4);
    gotoxy(45, 3);
    printf("MODIFICAR");

    if (lista == NULL) {
        gotoxy(35, 13);
        printf("La lista de alumnos esta vacia.");
        borrar();
        return;
    }

    int matriculaBusqueda;
    gotoxy(10, 6);
    printf("Ingrese la matricula del alumno que desea modificar: ");
    scanf("%d", &matriculaBusqueda);

    struct Nodo* actual = lista;

    while (actual != NULL && ((struct Alumno*)actual->datos)->matricula != matriculaBusqueda) {
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        gotoxy(15, 8);
        printf("No se encontraron datos pertenecientes a la matricula %d.\n", matriculaBusqueda);
    } else {
        int opcion;
        do {
        	margen(40, 2, 60, 4);
		    gotoxy(45, 3);
		    printf("MODIFICAR");
        	margen(1, 1, 100, 25);
        	color(hConsole, 11);
            gotoxy(30, 8);
            printf("Seleccione el dato a modificar:\n");
            gotoxy(7, 11);
            printf("1.");
            gotoxy(7, 12);
            printf("2.");
            gotoxy(7, 13);
            printf("3.");
            gotoxy(7, 14);
            printf("4.");
            color(hConsole, 7);
            imprimirDatosAlumno(actual->datos);
            gotoxy(7, 16);
            printf("0. Volver al menu principal");
            gotoxy(40, 18);
            printf("Opcion: ");
            scanf("%d", &opcion);
			system("cls");
			margen(1, 1, 100, 25);
            switch (opcion) {
                case 1:
                    gotoxy(30, 8);
                    printf("Nuevo nombre: ");
                    getchar(); //Espacios
                    fgets(((struct Alumno*)actual->datos)->nombre, sizeof(((struct Alumno*)actual->datos)->nombre), stdin);
                    ((struct Alumno*)actual->datos)->nombre[strcspn(((struct Alumno*)actual->datos)->nombre, "\n")] = '\0';
                    borrar();
                    break;

                case 2:
                    gotoxy(30, 8);
                    printf("Nuevo promedio: ");
                    scanf("%f", &((struct Alumno*)actual->datos)->promedio);
                    borrar();
                    break;

                case 3:
                    gotoxy(30, 8);
                    printf("Nuevo semestre: ");
                    scanf("%d", &((struct Alumno*)actual->datos)->semestre);
                    borrar();
                    break;

                case 4:
                    gotoxy(30, 8);
                    printf("Nueva carrera: ");
                    getchar();
                    fgets(((struct Alumno*)actual->datos)->carrera, sizeof(((struct Alumno*)actual->datos)->carrera), stdin);
                    ((struct Alumno*)actual->datos)->carrera[strcspn(((struct Alumno*)actual->datos)->carrera, "\n")] = '\0';
                    borrar();
					break;

                case 0:
                    break;

                default:
                    printf("Opcion no valida. Intente de nuevo.\n");
            }
        } while (opcion != 0);

        gotoxy(30, 20);
        printf("Datos modificados correctamente.\n");
    }
    borrar();
}

void guardar(struct Nodo* lista, size_t tamanoDatos, const char* nombreArchivo) {
    margen(1, 1, 100, 25);
    margen(40, 2, 60, 4);
    gotoxy(45, 3);
    printf("GUARDAR");
    
    if (lista == NULL) {
        gotoxy(30, 15);
        printf("La lista de alumnos esta vacia. No hay datos para guardar.");
        borrar();
        return;
    }

    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        gotoxy(30, 15);
        printf("Error al abrir el archivo para escritura.");
        borrar();
        return;
    }

    struct Nodo* actual = lista;

    while (actual != NULL) {
        fprintf(archivo, "Matricula: %d| Nombre: %s|Promedio: %.2f|Semestre: %d | Carrera: %s\n",
                ((struct Alumno*)actual->datos)->matricula,
                ((struct Alumno*)actual->datos)->nombre,
                ((struct Alumno*)actual->datos)->promedio,
                ((struct Alumno*)actual->datos)->semestre, 
                ((struct Alumno*)actual->datos)->carrera);

        actual = actual->siguiente;
    }

    fclose(archivo);
    gotoxy(30, 13);
    printf("Datos guardados correctamente en el archivo '%s'.", nombreArchivo);
    borrar();
}

void abrir(struct Nodo** lista, size_t tamanoDatos, const char* nombreArchivo) {
    margen(1, 1, 100, 25);
    margen(40, 2, 60, 4);
    gotoxy(45, 3);
    printf("ABRIR");

    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        gotoxy(30, 15);
        printf("No se pudo abrir el archivo '%s' para lectura.", nombreArchivo);
        borrar();
        return;
    }

    // Conservar los datos actuales en la lista
    struct Nodo* temp = *lista;
    *lista = NULL;

    // Leer y agregar nuevos datos desde el archivo
    char linea[100];
    int i = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Incrementar i antes de imprimir
        i++;
        // Imprimir las líneas del archivo
        gotoxy(5, 5 + i);
        printf("%s", linea);

        struct Alumno* alumno = (struct Alumno*)malloc(sizeof(struct Alumno));

        sscanf(linea, "Matricula: %d", &(alumno->matricula));

        // Busca la cadena "Nombre:" para obtener el dato
        char* nombreInicio = strstr(linea, "Nombre:") + strlen("Nombre: ");
        sscanf(nombreInicio, "%49[^|]", alumno->nombre);

        // Busca la cadena "Promedio:" para obtener el dato
        char* promedioInicio = strstr(linea, "Promedio:") + strlen("Promedio: ");
        sscanf(promedioInicio, "%f", &(alumno->promedio));

        // Busca la cadena "Semestre:" para obtener el dato
        char* semestreInicio = strstr(linea, "Semestre:") + strlen("Semestre: ");
        sscanf(semestreInicio, "%d", &(alumno->semestre));

        // Busca la cadena "Carrera:" para obtener el dato
        char* carreraInicio = strstr(linea, "Carrera:") + strlen("Carrera: ");
        sscanf(carreraInicio, "%49[^\n]", alumno->carrera);

        struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
        nuevoNodo->datos = alumno;
        nuevoNodo->siguiente = *lista;
        *lista = nuevoNodo;
    }

    // Libera memoria de los datos anteriores
    liberarMemoria(temp, free);

    fclose(archivo);
    gotoxy(30, 24);
    printf("Datos cargados correctamente desde el archivo '%s'.", nombreArchivo);
    borrar();
}


void liberarMemoria(struct Nodo* lista, void (*liberarDatos)(void*)) {
    while (lista != NULL) {
        struct Nodo* temp = lista;
        lista = lista->siguiente;

        liberarDatos(temp->datos);
        free(temp);
    }
}


void mostrarLista(struct Nodo* lista, size_t tamanoDatos, void (*imprimirDatos)(void*)) {
    printf("Lista de alumnos:\n");

    while (lista != NULL) {
        imprimirDatos(lista->datos);
        lista = lista->siguiente;
    }
    printf("NULL\n");
}

int margen(int a, int b, int c, int d) {
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	color(hConsole, 13);
    int j, k, i;
    j = a;
    gotoxy(a - 1, b);
    printf("%c", 201);
    gotoxy(a - 1, d);
    printf("%c", 200);
    for (a; a <= c; a++) {
        gotoxy(a, b);
        printf("%c", 205);
        gotoxy(a, d);
        printf("%c", 205);
    }
    gotoxy(a, b);
    printf("%c", 187);
    gotoxy(a, d);
    printf("%c", 188);
    k = b + 1;
    for (k; k <= (d - 1); k++) {
        gotoxy(j - 1, k);
        printf("%c", 186);
        gotoxy(a, k);
        printf("%c", 186);
    }
	color(hConsole, 7);
}
void gotoxy(int x, int y) {
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

char menu() {
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	margen(1, 1, 100, 25);
    int x = 15, y = 8;
    int n = 59, m = 9;
    char pla;
    margen(49, 4, 54, 6);
    margen(10, 7, 50, 18 );
    
    margen(59, 7, 95, 18);
    color(hConsole, 11);
		gotoxy(75, 8 );
		printf("NOTA");
		gotoxy(60, 10);
		printf("Las teclas de direccion del teclado");
		gotoxy(61, 11);
		printf("permiten la navegacion en el menu.");
		gotoxy(60, 13);
		printf("Para acceder a las opciones ingrese");
		gotoxy(65, 14);
		printf("la letra correspondiente.");
    
    
    gotoxy(50, 5);
	printf("MENU");
    gotoxy(x, y + 1);
    printf("Alta [A]......................[   ]");
    gotoxy(x, y + 2);
    printf("Baja [B]......................[   ]");
    gotoxy(x, y + 3);
    printf("Consulta [C]..................[   ]");
    gotoxy(x, y + 4);
    printf("Modificaciones [M]............[   ]");
    gotoxy(x, y + 5);
    printf("Ordenar[O]....................[   ]");
    gotoxy(x, y + 6);
    printf("Guardar [G]...................[   ]");
    gotoxy(x, y + 7);
    printf("Abrir [L].....................[   ]");
    gotoxy(x, y + 8);
    printf("Salir [S ]....................[   ]");
	
    do {
        gotoxy(47, m);
            pla = _getch();//Lee las teclas/numeros de direccion
            switch (pla) {
                case 72:  // Arriba
                    if (m > 9) m--;
                    break;
                case 80:  // Abajo
                    if (m < 16) m++;
                    break;
            }
    } while (pla != 'A' && pla != 'B' && pla != 'C' && pla != 'M' && pla != 'O' && pla != 'G' && pla != 'L' && pla != 'S');
    return pla;
}
void ordenar(struct Nodo** lista, size_t tamanoDatos, int (*compararDatos)(const void*, const void*)) {
    margen(1, 1, 100, 25);
    margen(40, 2, 60, 4);
    gotoxy(45, 3);
    printf("ORDENAR");

    if (*lista == NULL) {
        gotoxy(40, 13);
        printf("La lista se encuentra vacia");
        borrar();
        return;
    }

    if ((*lista)->siguiente == NULL) {
        gotoxy(15, 13);
        printf("La lista contiene 1 elemento, ya se encuentra ordenada.");
        borrar();
        return;
    }

    int intercambio;
    struct Nodo* actual;
    struct Nodo* siguiente = NULL;

    do {
        intercambio = 0;
        actual = *lista;

        while (actual->siguiente != siguiente) {
            if (compararDatos(actual->datos, actual->siguiente->datos) > 0) {
                swap(actual, actual->siguiente);
                intercambio = 1;
            }
            actual = actual->siguiente;
        }
        siguiente = actual;
    } while (intercambio);

    gotoxy(30, 13);
    printf("La lista ha sido ordenada por matricula");
    borrar();
}
void swap(struct Nodo* a, struct Nodo* b) {
    void* temp = a->datos;
    a->datos = b->datos;
    b->datos = temp;
}
void mostrarMatriculas(struct Nodo* lista) {
    system("cls");  // Borrar pantalla antes de imprimirlas
    int i = 0;
    int x = 1;

    while (lista != NULL) {
        struct Alumno* alumno = (struct Alumno*)lista->datos;

        gotoxy(x, 20);
        printf("%c", 218);
        gotoxy(x, 21);
        printf("%c", 179);
        gotoxy(x, 22);
        printf("%c", 192);

        int lon = snprintf(NULL, 0, "%d", alumno->matricula); // Calcular longitud del número
        char matriculaStr[lon + 1]; 
        snprintf(matriculaStr, sizeof(matriculaStr), "%d", alumno->matricula);

        for (i = 0; i < lon + 2; i++) {
            gotoxy(x + i + 1, 20);
            printf("%c", 196);
            gotoxy(x + i + 1, 21);
            printf("%c", matriculaStr[i]);
            gotoxy(x + i + 1, 22);
            printf("%c", 196);
        }

        gotoxy(x + lon + 1, 20);
        printf("%c", 194);
        gotoxy(x + lon + 1, 21);
        printf("%c", 179);
        gotoxy(x + lon + 1, 22);
        printf("%c", 193);

        gotoxy(x + lon + 3, 20);
        printf("%c", 191);
        gotoxy(x + lon + 3, 21);
        printf("%c", 179);
        gotoxy(x + lon + 3, 22);
        printf("%c", 217);

        lista = lista->siguiente;

        if (lista != NULL) {
            gotoxy(x + lon + 4, 21);
            printf("->");
        }

        x += (lon + 6);
    }
}

void imprimirDatosAlumno(void* datos) {
    struct Alumno* alumno = (struct Alumno*)datos;
    gotoxy(10, 10);
    printf("Matricula: %d", alumno->matricula);
    gotoxy(10, 11);
    printf("Nombre: %s", alumno->nombre);
    gotoxy(10, 12);
    printf("Promedio: %.2f", alumno->promedio);
    gotoxy(10, 13);
    printf("Semestre: %d", alumno->semestre);
    gotoxy(10, 14);
    printf("Carrera: %s", alumno->carrera);
}

void borrar()
{
	gotoxy(60,25);
	printf("Presione una tecla para continuar...");
	getch();
	system("cls");
}

