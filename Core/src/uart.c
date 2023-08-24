#include "uart.h"
#include "log.h"
static uint32_t receive_flag;
static uint32_t rx_count, rx_size;
/*!
    \brief      configure the USART1 GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart1_gpio_config(void) {
    /* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
}

/*!
    \brief      configure the USART1
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart1_config(void) {
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    /* USART configure */
    usart_deinit(USART1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_baudrate_set(USART1, 115200);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(USART1, USART_DENT_ENABLE);
    usart_dma_receive_config(USART1, USART_DENR_ENABLE);

    usart_enable(USART1);
}

static void usart1_dma_config() {
    dma_parameter_struct dma_init_struct;
    rcu_periph_clock_enable(RCU_DMA);

    dma_deinit(USART1_DMA_TX_CHx);

    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = 0;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = 0;
    dma_init_struct.periph_addr = (uint32_t)&USART_TDATA(USART1);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init(USART1_DMA_TX_CHx, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(USART1_DMA_TX_CHx);
    dma_memory_to_memory_disable(USART1_DMA_TX_CHx);
    /* enable DMA channel1 */
    dma_channel_disable(USART1_DMA_TX_CHx);

    /*设置接收DMA*/
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = 0;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = 0;
    dma_init_struct.periph_addr = (uint32_t)&USART_RDATA(USART1);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(USART1_DMA_RX_CHx, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(USART1_DMA_RX_CHx);
    /* enable DMA channel2 */
    dma_channel_enable(USART1_DMA_RX_CHx);
}
void Usartx_Init() {
    nvic_irq_enable(USART1_IRQn, 1, 0);
    usart1_gpio_config();
    usart1_config();
    usart1_dma_config();

    usart_flag_clear(USART1, USART_FLAG_IDLE);
}

void UART_Transmit_DMA(uint32_t usart_periph, uint32_t p_data, uint32_t data_Size) {
    if (usart_periph == USART1) {
        dma_channel_disable(USART1_DMA_TX_CHx);
        dma_flag_clear(USART1_DMA_TX_CHx, DMA_FLAG_G);
        dma_memory_address_config(USART1_DMA_TX_CHx, p_data);
        dma_transfer_number_config(USART1_DMA_TX_CHx, data_Size);
        while (RESET == usart_flag_get(USART1, USART_FLAG_TBE))
            ;
        // usart_dma_transmit_config(USART1, USART_DENT_ENABLE);
        dma_channel_enable(USART1_DMA_TX_CHx);
        while (RESET == dma_flag_get(USART1_DMA_TX_CHx, DMA_FLAG_FTF)) {
        }
    }
}

void UART_Receive_DMA(void* buf, uint32_t size) {
    dma_channel_disable(USART1_DMA_RX_CHx);
    dma_transfer_number_config(USART1_DMA_RX_CHx, size);
    dma_memory_address_config(USART1_DMA_RX_CHx, (uint32_t)buf);
    dma_channel_enable(USART1_DMA_RX_CHx);
    usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);
    usart_interrupt_enable(USART1, USART_INT_IDLE);

    rx_size = size;
}

void USART1_IRQHandler(void) {
    // LOG("USART1_IRQHandler");
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE)) {
        char* s;
        /* clear IDLE flag */
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);

        /* number of data received */
        s = (char*)DMA_CHMADDR(USART1_DMA_RX_CHx);
        rx_count = rx_size - (dma_transfer_number_get(USART1_DMA_RX_CHx));
        s[rx_count + 1] = 0;
        receive_flag = 1;

        /* disable DMA and reconfigure */
        dma_channel_disable(USART1_DMA_RX_CHx);
        dma_transfer_number_config(USART1_DMA_RX_CHx, rx_size);
        dma_channel_enable(USART1_DMA_RX_CHx);
    }
}

uint8_t USART1_Get_Rx(char** addr, uint32_t* size) {
    *addr = (char*)DMA_CHMADDR(USART1_DMA_RX_CHx);
    *size = rx_count;
    if (receive_flag == 0) {
        return 1;
    }
    receive_flag = 0;
    return 0;
}