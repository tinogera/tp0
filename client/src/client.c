#include "client.h"

int main(void)
{
	
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	config = iniciar_config();

	config = config_create("cliente.config");
	if(config==NULL) {
		log_error(logger,"No se pudo crear el config");
		log_destroy(logger);
		abort();
	}
	
	valor = config_get_string_value(config,"CLAVE");

	log_info(logger, valor);

	leer_consola(logger);


	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("cliente.log", "cliente", true, LOG_LEVEL_INFO);;

	if(nuevo_logger==NULL) abort();

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	leido = readline("> ");

	while (leido != NULL && leido[0] != '\0')
	{
		log_info(logger, leido);
		free(leido);
		leido= readline(">");
	}
	free(leido);

}

void paquete(int conexion)
{
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while(leido!=NULL&&leido[0]!='\0'){
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);
        leido = readline("> ");
	}

	free(leido);
    enviar_paquete(paquete, conexion);
    eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
    log_destroy(logger);
    config_destroy(config);
}
