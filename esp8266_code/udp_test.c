
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include <lwip/udp.h>
#include <lwip/tcp.h>
#include <stdint.h>

#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
#elif (SPI_FLASH_SIZE_MAP == 3)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 4)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#else
#error "The flash map is not supported"
#endif

static const partition_item_t at_partition_table[] = {
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};

static os_timer_t some_timer;

void ICACHE_FLASH_ATTR user_pre_init(void)
{
  if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
    os_printf("system_partition_table_regist fail\r\n");
    while(1);
	}
}

os_timer_t msg_timer;
os_timer_t shutdown_timer;
struct udp_pcb *ptel_pcb = NULL;

char msg[] = "testing";
static bool msg_sent = false;

static struct tcp_pcb * tcp_conn = NULL;
static unsigned sent_called = 0;


void ICACHE_FLASH_ATTR shutdown_timer_cb(void* unused)
{
  system_deep_sleep(10e6);
}

void ICACHE_FLASH_ATTR send_udp_packet(void)
{
  // struct ip_addr ip;
  // IP4_ADDR(&ip, 192, 168, 87, 13);

  struct pbuf *p;
  p = pbuf_alloc(PBUF_TRANSPORT,sizeof(msg),PBUF_RAM);
  memcpy (p->payload, msg, sizeof(msg));
  udp_send(ptel_pcb, p);
  pbuf_free(p); //De-allocate packet buffer
}

void ICACHE_FLASH_ATTR receive_udp_packet(void * arg, struct udp_pcb * pcb, struct pbuf * packet, struct ip_addr * addr, u16_t port)
{
  char tmp[1024];
  u16_t length = pbuf_copy_partial(packet, tmp, sizeof(tmp) - 1, 0);
  tmp[length] = '\0';
  os_printf("From %s:%u => %s\r\n", ipaddr_ntoa(addr), port, tmp);
  pbuf_free(packet);

    os_timer_setfn(&shutdown_timer, shutdown_timer_cb, NULL);
    os_timer_arm(&shutdown_timer, 10, false);
}

// err_t ICACHE_FLASH_ATTR tcp_connected_cb(void * arg, struct tcp_pcb * conn, err_t err)
// {
//   char local_ip[32];
//   char remote_ip[32];

//   ipaddr_ntoa_r(&conn->local_ip, local_ip, sizeof(local_ip));
//   ipaddr_ntoa_r(&conn->remote_ip, remote_ip, sizeof(remote_ip));
//   os_printf("TCP connected %s:%u -> %s:%u\r\n", local_ip, ntohs(conn->local_port), remote_ip, ntohs(conn->remote_port));

//   char msg[] = "Hello there\r\n";
//   tcp_write(conn, msg, sizeof(msg) - 1, 0);
//   tcp_output(conn);

//   os_printf("Send complete\r\n");
// }

// err_t ICACHE_FLASH_ATTR tcp_sent_cb(void * arg, struct tcp_pcb * conn, u16_t len)
// {
//   ++sent_called;
//   os_printf("Sent CB invoked %u times\r\n", sent_called);

//   tcp_close(conn);
// }

// err_t ICACHE_FLASH_ATTR tcp_recv_cb(void * arg, struct tcp_pcb * conn, struct pbuf * p, err_t err)
// {
//   os_printf("TCP recv CB called with buffer %p\r\n", p);

//   if (!p)
//   {
//     os_timer_setfn(&shutdown_timer, shutdown_timer_cb, NULL);
//     os_timer_arm(&shutdown_timer, 10, false);
//   }
// }

// void ICACHE_FLASH_ATTR tcp_error_cb(void * arg, err_t err)
// {
//   os_printf("Error from TCP stack: %d\r\n", err);
//   tcp_close(arg);
// }

// void ICACHE_FLASH_ATTR send_tcp_msg(void)
// {
//   struct ip_addr ip;
//   IP4_ADDR(&ip, 192, 168, 87, 13);

//   tcp_conn = tcp_new();
//   if (tcp_conn == NULL)
//   {
//     os_printf("Failed to create TCP connection\r\n");
//     return;
//   }

//   tcp_arg(tcp_conn, tcp_conn);
//   tcp_err(tcp_conn, tcp_error_cb);
//   tcp_sent(tcp_conn, tcp_sent_cb);
//   tcp_recv(tcp_conn, tcp_recv_cb);
  
//   // err_t err = tcp_bind(conn, IP4_ADDR_ANY, 0);
//   // if (err != ERR_OK)
//   // {
//   //   os_printf("Error binding TCP connection: %d\r\n", err);
//   //   goto error;
//   // }

//   err_t err = tcp_connect(tcp_conn, &ip, 5555, tcp_connected_cb);
//   if (err != ERR_OK)
//   {
//     os_printf("Error connecting to remote host: %d\r\n", err);
//     tcp_close(tcp_conn);
//     tcp_conn = NULL;
//   }
// }

void ICACHE_FLASH_ATTR wifi_status_update_cb(System_Event_t * evt)
{
  if (evt->event == EVENT_STAMODE_GOT_IP)
  {
    os_printf("Got IP\r\n");

    struct ip_addr ip;
    IP4_ADDR(&ip, 192, 168, 87, 13);

    ptel_pcb = udp_new();
    if (ptel_pcb == NULL)
    {
      os_printf("Error allocating UDP connection\r\n");
      return;
    }

    err_t err = udp_connect(ptel_pcb, &ip, 5000);
    if (err != ERR_OK)
    {
      os_printf("Error connecting to remote UDP port: %d\r\n", err);
      goto error;
    }

    udp_recv(ptel_pcb, receive_udp_packet, NULL);
    send_udp_packet();
    // send_tcp_msg();

    return;

  error:
    udp_remove(ptel_pcb);
  }

}

void ICACHE_FLASH_ATTR init_done(void)
{
  os_printf("Init done called\r\n");
  os_printf("Hello there stranger\r\n");
  // wifi_fpm_do_sleep(UINT32_MAX);
}

void ICACHE_FLASH_ATTR user_init(void)
{
  uart_div_modify(0, UART_CLK_FREQ / 115200);

  wifi_set_opmode(NULL_MODE);
  wifi_station_set_auto_connect(0);

  wifi_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();

  system_timer_reinit();
  system_init_done_cb(init_done);

  // const char ssid[32] = "mujido-sense";
  // const char password[64] = "mzbke4wKlyhnScDQPyixRHy35DxpxUeVYNWZj3Y5lVFZf0tjg5FBbMEd7wGgldp";

  // wifi_set_opmode(1);

  // struct station_config station_conf;

  // station_conf.bssid_set = 0;
  // os_memcpy(&station_conf.ssid, ssid, sizeof(ssid));
  // os_memcpy(&station_conf.password, password, sizeof(password));
  // wifi_station_set_config(&station_conf);
  // wifi_set_event_handler_cb(wifi_status_update_cb);
}