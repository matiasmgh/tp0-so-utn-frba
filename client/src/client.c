#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char *ip;
	char *puerto;
	char *valor;

	t_log *logger;
	t_config *config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "hola!");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	log_info(logger, "Iniciando la configuracion...");
	config = iniciar_config(logger);

	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	if (config != NULL)
	{
		if (config_has_property(config, "CLAVE"))
		{
			valor = config_get_string_value(config, "CLAVE");
			ip = config_get_string_value(config, "IP");
			puerto = config_get_string_value(config, "PUERTO");
		} else {
			exit(3);
		log_error(logger, "No se pudo acceder a la key CLAVE de la configuracion.");
		}
	} else {
		exit(3);
		log_error(logger, "No se pudo acceder al archivo de configuracion.");
	}

	// Loggeamos el valor de config

	log_info(logger, valor, "%s");
	log_info(logger, ip, "%s");
	log_info(logger, puerto, "%s");

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	char *clave = config_get_string_value(config, "CLAVE");
	// send(conexion, &clave, sizeof(clave), NULL);
	enviar_mensaje(clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log *iniciar_logger(void)
{
	t_log *nuevo_logger;

	nuevo_logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config *iniciar_config(t_log *logger)
{
	t_config *nuevo_config;

	nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL) {
		log_error(logger, "No se pudo hallar el archivo de configuracion en el path.");
	}

	return nuevo_config;
}

void leer_consola(t_log *logger)
{
	char *leido;

	// La primera te la dejo de yapa
	log_info(logger, "Leyendo de consola. Inserte lo que desee loguear o vacio para continuar.");
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(leido[0] != '\0') {
		log_info(logger, leido, "%s");
		leido = readline("> ");
	}

	log_info(logger, "Continuando...");

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(0);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char *leido;
	t_paquete *paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while(leido[0] != '\0') {
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log *logger, t_config *config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	close(conexion);
	log_destroy(logger);
	config_destroy(config);
}
