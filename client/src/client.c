#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char* ruta_logs = "../tp0.log"; 
char* nombre_proceso_logs = "Cliente";

int main(void)
{

	char cwd[1024]; // Se reserva espacio para el directorio de trabajo actual

    if (getcwd(cwd, sizeof(cwd)) != NULL) { // Se intenta obtener el directorio de trabajo actual
        printf("Directorio de trabajo actual: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
	
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;
	t_log* logger;
	t_config* config;
	
	char *readline(const char *prompt);
	

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();	
	

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = config_get_string_value(config , "CLAVE");
	ip = config_get_string_value(config , "IP");
	puerto = config_get_string_value(config , "PUERTO");

	// Loggeamos el valor de config

	log_info(logger, valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);
	

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él
	printf("ip = %s\n", ip);
   	printf("puerto = %s\n", puerto);


	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje


	enviar_mensaje(valor, conexion);


     
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{

	t_log* nuevo_logger;

	nuevo_logger = log_create(ruta_logs, nombre_proceso_logs, true, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL) {
        printf("Error al crear el logger.\n");
        return 1; // Termina el programa con un código de error
    }

	// Usando el logger creado previamentegit 
	// Escribi: "Hola! Soy un log"

	log_info(nuevo_logger, "HOla! Soy un log");

	return nuevo_logger;


}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	
	nuevo_config = config_create("../cliente.config");
	if (nuevo_config == NULL) {
		printf("Error al cargar la configuración desde el archivo.\n");
    // ¡No se pudo crear el config!
    // Terminemos el programa
	}

	return nuevo_config;

}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

	while (1) {
		leido = readline("> ");

		add_history(leido);

		if (!strncmp(leido, "\0", 1)) {
			break;
		}

		printf("%s\n", leido);
		log_info(logger, leido);

	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
	
}


	



void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();

	while (1) {
			
			leido = readline("> ");

			agregar_a_paquete(paquete, leido, strlen(leido) + 1);

			if (!strncmp(leido, "\0", 1)) {
				free(leido);
				break;
			}
		
		}

    // Enviar el paquete
    enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
