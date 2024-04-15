#include "udp_handler.h"

#include "udp.h"

#include <string.h>

static struct udp_pcb* upcb = NULL;

static void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p,
	    const ip_addr_t *addr, u16_t port)
{
	// в этой функции обязательно должны очистить p, иначе память потечёт
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
	pbuf_free(p);
}

err_t udp_create_socket()
{
	// проверяем, что не инициализировали сокет еще
	if (upcb == NULL)
	{
		// создание сокета
		upcb = udp_new();

		// если не удалось создать сокет, то на выход с ошибкой
		if (upcb == NULL)
		{
			return ERR_ABRT;
		}
	}

	ip4_addr_t dest;
	IP4_ADDR(&dest, 169, 254, 248, 52);
	// коннектимся к удаленному серверу по ИП и порту (сервер должен быть настроен именно на так)
	err_t err = udp_connect(upcb, &dest, 3333);
	if (ERR_OK != err)
	{
		return err;
	}

	// регистрируем колбэк на прием пакета
	udp_recv(upcb, udp_receive_callback, NULL);
	return ERR_OK;
}

err_t udp_send_msg(char* message, int n)
{
	// если сокет не создался, то на выход с ошибкой
	if (upcb == NULL)
	{
		return ERR_ABRT;
	}
	// аллоцируем память под буфер с данными
	struct pbuf* p = pbuf_alloc(PBUF_TRANSPORT, n, PBUF_RAM);
	err_t err = ERR_OK;
	if (p != NULL)
	{
		// кладём данные в аллоцированный буфер
		err = pbuf_take(p, message, n);
		if (err==ERR_OK)
		{
			// отсылаем пакет
			err = udp_send(upcb, p);
		}
		pbuf_free(p);
	}
	return err;
}
