#include <multiboot.h>
#include <system.h>
#include <printf.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <vga.h>
#include <timer.h>
#include <asciiart.h>
#include <pmm.h>
#include <paging.h>
#include <kheap.h>
#include <pci.h>
#include <ata.h>
#include <vfs.h>
#include <string.h>
#include <ext2.h>
#include <process.h>
#include <usermode.h>
#include <syscall.h>
#include <elf_loader.h>
#include <mouse.h>
#include <keyboard.h>
#include <font.h>
#include <rtc.h>
#include <rtl8139.h>
#include <ethernet.h>
#include <ip.h>
#include <udp.h>
#include <dhcp.h>
#include <serial.h>
#include <spinlock.h>
#include <kmain.h>
#include <string.h>


extern ata_dev_t primary_master;
extern datetime_t current_datetime;

#define MSIZE 48 * M
#define NETWORK_MODE 0
#define PROMPT "~"

void start_shell();
void completed_init();

char *current_directory;
int cwd_len;

char *keyboard_input;
int kbdi_len;

int shift = 0;

void on_keypress(int kc, char c) {

    if (kc == RSHIFT || kc == LSHIFT) {
        shift = 1;
    }

    if (shift == 1) {
        if (c == 'q') c = 'Q';
        if (c == 'w') c = 'W';
        if (c == 'e') c = 'E';
        if (c == 'r') c = 'R';
        if (c == 't') c = 'T';
        if (c == 'y') c = 'Y';
        if (c == 'u') c = 'U';
        if (c == 'i') c = 'I';
        if (c == 'o') c = 'O';
        if (c == 'p') c = 'P';

        if (c == 'a') c = 'A';
        if (c == 's') c = 'S';
        if (c == 'd') c = 'D';
        if (c == 'f') c = 'F';
        if (c == 'g') c = 'G';
        if (c == 'h') c = 'H';
        if (c == 'j') c = 'J';
        if (c == 'k') c = 'K';
        if (c == 'l') c = 'L';

        if (c == 'z') c = 'Z';
        if (c == 'x') c = 'X';
        if (c == 'c') c = 'C';
        if (c == 'v') c = 'V';
        if (c == 'b') c = 'B';
        if (c == 'n') c = 'N';
        if (c == 'm') c = 'M';

        shift = 0;
    }

    printf((const char*) c);
    strcat(keyboard_input, (char*) c);
    kbdi_len++;
}

int kmain(multiboot_info_t * mb_info) {

    current_directory = "/";
    cwd_len = strlen(current_directory);

    completed_init();

    process_init();
    syscall_init();

    // Set TSS stack so that when process return from usermode to kernel mode, the kernel have a ready-to-use stack
    //uint32_t esp;
    //asm volatile("mov %%esp, %0" : "=r"(esp));
    //tss_set_stack(0x10, esp);

    // Start the first process
    //create_process_from_routine(user_process, "user process");

    //qemu_printf("\nDone!\n");

    clear();
    start_shell();

    return 0;
}


/*
 * Move all the init code to here, so the kmain function is more clean and easier to read
 * */
void completed_init() {
    video_init();
    qemu_printf("%s\n", simpleos_logo);

    // Initialize everything (green)
    set_curr_color(LIGHT_GREEN);
    qemu_printf("Initializing video(text mode 80 * 25)...\n");

    qemu_printf("Initializing gdt, idt and tss...\n");
    gdt_init();
    idt_init();
    tss_init(5, 0x10, 0);

    qemu_printf("Initializing physical memory manager...\n");
    pmm_init(1096 * M);

    qemu_printf("Initializing paging...\n");
    paging_init();

    qemu_printf("Initializing kernel heap...\n");
    kheap_init(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX_ADDRESS);

    qemu_printf("Initializing timer...\n");
    timer_init();

    qemu_printf("Initializing pci...\n");
    pci_init();

    qemu_printf("Initializing keyboard...\n");
    keyboard_init();

    qemu_printf("Initializing vfs, ext2 and ata/dma...\n");
    vfs_init();
    ata_init();
    ext2_init("/dev/hda", "/");


    qemu_printf("Initializing real time clock...\n");
    rtc_init();
    qemu_printf("Current date and time: %s\n", datetime_to_str(&current_datetime));
    #if NETWORK_MODE
    qemu_printf("Initializing network driver...\n");
    rtl8139_init();
    arp_init();

    uint8_t mac_addr[6];
    mac_addr[0] = 0xAA;
    mac_addr[1] = 0xBB;
    mac_addr[2] = 0xCC;
    mac_addr[3] = 0xDD;
    mac_addr[4] = 0xEE;
    mac_addr[5] = 0xFF;
    get_mac_addr(mac_addr);

    uint8_t ip_addr[6];
    ip_addr[0] = 10;
    ip_addr[1] = 0;
    ip_addr[2] = 2;
    ip_addr[3] = 15;
    char * str = "This is a message sent from simpleos to an Ubuntu host";
    //ethernet_send_packet(mac_addr, str, strlen(str), 0x0021);
    //ip_send_packet(ip_addr, str, strlen(str));

    // Ask QEMU's dhcp server for an IP address
    dhcp_discover();
    // If IP is ready, send a UDP message from simpleos to a host machine running Ubuntu
    while(gethostaddr(mac_addr) == 0);
    udp_send_packet(ip_addr, 1234, 1153, str, strlen(str));
    for(;;);
    #endif
}

void start_shell() {
    for (;;) {
        printf(current_directory);
        printf(" ");
        printf(PROMPT);
        printf(" ");
    }
}