#include <stdio.h>
#include <stdlib.h>
#include "pila.h"
#include <string.h>

typedef struct
{
    char nombreYapellido[50];
    char especialidad[50];
    int matricula;
    float sueldo;
}stMedico;

//creo una estructura de especialidades
typedef struct
{
    char especialidad[50];
}stEspecialidades;

////////////////////////////// PROTOTIPADOS ///////////////////////////////////////
// 1.
int verificarMatricula(stMedico doctores[], int validos, int matricula);
// 2.
stMedico cargaUnMedico(stMedico doctores[], int validos);
int cargarArrayMedicos(stMedico doctor[]);
// 3.
void archivoEspecialidad(char especialidad[], stMedico doctores[], int validos);
// 4.
int verificoEspecialidad(stEspecialidades cargadas[], int validoCargadas, char esp[]);
int medicosAarchivos(stMedico doctores[], int validos, stEspecialidades cargadas[]);
// 5.
void mostrarUnMedico(stMedico doctor);
void mostrarArchivo(char nombre[]);
//6.
float mayorSueldo(char nombre[]);

////////////////////////////////// MAIN //////////////////////////////////////////

int main()
{
    stMedico doctores[50];
    int validosDoctores=0;
    stEspecialidades esp[50];
    int validosEspecialidades=0;

    char archi[50];
    char ext[]=".info";
    char nombreEsp[50];
    float mayor;

    validosDoctores=cargarArrayMedicos(doctores);

    //paso el array a archivos
    validosEspecialidades=medicosAarchivos(doctores,validosDoctores,esp);

    for(int i=0; i<validosEspecialidades; i++)
    {
        //guardo la especialidad actual en un array
        strcpy(archi,esp[i].especialidad);
        //lo paso a minuscula
        strlwr(archi);
        //le agrego la extencion
        strcat(archi,ext);

        strcpy(nombreEsp,esp[i].especialidad);
        strupr(nombreEsp);

        printf("\n---------------------------------\n");
        printf("ESPECIALIDAD: %s\n", nombreEsp);
        printf("---------------------------------\n");

        mostrarArchivo(archi);

        mayor=mayorSueldo(archi);
        printf("\nEl mayor sueldo de esta especialidad es : $%.2f\n\n", mayor);
    }


    return 0;
}

////////////////////////////////// FUNCIONES //////////////////////////////////////////

/** 1.
Hacer una función que verifique si existe una matrícula en un arreglo de stMedico.
La función devolverá 0 o 1 según corresponda. 15 pts.
**/
int verificarMatricula(stMedico doctores[], int validos, int matricula)
{
    int flag=0;//inicializo en 0 --- no hay ninguno encontrado
    int i=0;//contador

    while((i<validos)&&(flag==0))
    {
        if(doctores[i].matricula==matricula)
        {
            flag=1;//si encuentra una matricula igual sale del ciclo y retorna 1
        }

        i++;//incremento el contador
    }


    return flag;//si encuentra uno igual retorna 1, sino retorna 0
}

/** 2.
Hacer una función que cargue médicos en un arreglo. Realizar las comprobaciones
pertinentes tanto de sueldo (mayor a 0) y que la matrícula no exista.
**/

stMedico cargaUnMedico(stMedico doctores[], int validos)
{
    //declaro una variable de tipo stMedico
    stMedico aux;
    int flag;

    printf(" ------------------------\n");
    printf("|   CARGANDO UN MEDICO   |\n");
    printf(" ------------------------\n");

    printf("\nIngrese el Nombre y Apellido: ");
    fflush(stdin);//limpio el buffer
    gets(aux.nombreYapellido);

    printf("\nIngrese la especialidad: ");
    fflush(stdin);
    gets(aux.especialidad);

    do
    {
        printf("\nIngrese la matricula: ");
        fflush(stdin);
        scanf("%d", &aux.matricula);

        //verifico si existe
        flag=verificarMatricula(doctores,validos,aux.matricula);

        if(flag==1)
        {
            //si la matricula existe le muestro un mensaje de error
            printf("\nERROR: la matricula ingresada ya existe. Vuelva a ingresarla.\n");
            system("pause");//pausa antes de volver a pedir los datos
        }

        //el ciclo continua hasta que la matricula sea diferente de las que estan cargadas
    }while(flag==1);


    do
    {
        printf("\nIngrese el sueldo: $");
        fflush(stdin);
        scanf("%f", &aux.sueldo);

        if(aux.sueldo<=0)
        {
            //si el sueldo ingresado es menor o igual a 0 muestro un mensaje de error
            printf("\nERROR: el sueldo no puede ser menor o igual a 0. Vuelva a ingresarlo\n");
            system("pause");//pausa antes de volver a pedir los datos
        }
    }while(aux.sueldo<=0);

    return aux;//devuelve un medico cargado
}

int cargarArrayMedicos(stMedico doctor[])
{
    int validos=0;
    char mander;//para seguir con la carga de medicos

    do
    {
        doctor[validos]=cargaUnMedico(doctor,validos);//carga un medico al arreglo
        validos++;//incremento los validos

        printf("\nDesea seguir cargando medicos? (s/n)\n");
        fflush(stdin);
        scanf("%c", &mander);
        system("cls");//limpia la pantalla
    }while(mander=='s');

    return validos;//devuelve los validos
}

/** 3.
Hacer una función que grabe en un archivo todos los médicos de una determinada
especialidad enviada por parámetro. El nombre del archivo deberá ser la especialidad.
**/
void archivoEspecialidad(char esp[], stMedico doctores[], int validos)
{
    char extencion[] = ".info";//extencion

    //declaro auxiliares para comparar
    char espArray[50];
    char espArchi[50];

    FILE * archi;

    //paso la especialidad a minuscula
    strlwr(esp);

    strcpy(espArchi,esp);

    //concateno la especialidad con la extencion
    strcat(espArchi,extencion);

    archi=fopen(espArchi,"ab");

    if(archi!=NULL)
    {
        for(int i=0; i<validos; i++)
        {
            //paso la esecialidad del array a otro array y luego a minuscula para comparar mejor
            strcpy(espArray,doctores[i].especialidad);
            strlwr(espArray);

            if(strcmp(esp, espArray)==0)
            {
                //si la especialidad es igual a la que esta buscando
                //lo escribe en el archivo
                fwrite(&doctores[i],sizeof(stMedico),1,archi);
            }
        }
    }
    fclose(archi);
}

/** 4.
Hacer una función que pase todos los médicos del arreglo a varios archivos utilizando
la función anterior.
**/

int verificoEspecialidad(stEspecialidades cargadas[], int validoCargadas, char esp[])
{
    int flag=0;//inicializo en no encontrado
    int i=0;//contador

    char espArray[50];

    strlwr(esp);//paso la especialidad a minuscula

    do
    {
        strcpy(espArray,cargadas[i].especialidad);//copio la especialidad actual a otro string
        strlwr(espArray);//lo paso a minuscula

        if(strcmp(espArray,esp)==0)
        {
            //si son iguales
            flag=1;
        }
        i++;//incremento el contador
    }while((i<validoCargadas)&&(flag==0));

    return flag;//devuelve si lo encontro o no
}

int medicosAarchivos(stMedico doctores[], int validos, stEspecialidades cargadas[])
{
    int flag;
    int validoCargadas=0;

    for(int i=0; i<validos; i++)
    {
        //primero verifico que la especialidad que estoy viendo no esta cargada
        flag=verificoEspecialidad(cargadas, validoCargadas, doctores[i].especialidad);
        if(flag==0)
        {
            //si no encontro ninguna especialidad igual ya cargada
            archivoEspecialidad(doctores[i].especialidad,doctores,validos);

            //guardo una nueva especialidad cargada
            strcpy(cargadas[validoCargadas].especialidad,doctores[i].especialidad);
            validoCargadas++;//incremento las especialidades cargadas
        }

    }
    return validoCargadas;
}

/** 5.
Hacer una función para mostrar un archivo.
**/
void mostrarUnMedico(stMedico doctor)
{
    printf("\n-----------------------------------------------------------------\n");
    printf("Nombre y Apellido: .......................%s\n", doctor.nombreYapellido);
    printf("Especialidad : ...........................%s\n", doctor.especialidad);
    printf("Matricula : ..............................%d\n", doctor.matricula);
    printf("Sueldo : .................................$%.2f\n", doctor.sueldo);
}
void mostrarArchivo(char nombre[])
{
    stMedico aux;

    FILE * archi;

    archi=fopen(nombre,"rb");

    if(archi!=NULL)
    {
        while(fread(&aux,sizeof(stMedico),1,archi)>0)
        {
            mostrarUnMedico(aux);//muestra un medico
        }
    }

    fclose(archi);
}

/** 6.
Hacer un función que devuelva el mayor sueldo de los médicos del archivo.
**/
float mayorSueldo(char nombre[])
{
    float mayor=0;
    stMedico aux;

    FILE * archi;

    archi=fopen(nombre,"rb");

    if(archi!=NULL)
    {
        while(fread(&aux, sizeof(stMedico),1,archi)>0)
        {
            if((mayor==0)||(aux.sueldo>mayor))
            {
                //si no hay ningun mayor cargado
                //o si el suelto del actual es mayor a mayor
                mayor=aux.sueldo;
            }
        }
    }

    fclose(archi);

    return mayor;
}
