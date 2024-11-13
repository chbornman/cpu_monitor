#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/processor_info.h>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>

// Function to clear screen
void clearScreen() {
    printf("\033[2J");   // Clear screen
    printf("\033[H");    // Move cursor to home position
}

// Function to get CPU usage for each core
void getCPUUsage(float *cpu_usage, int cpu_count) {
    natural_t processor_count;
    processor_cpu_load_info_t cpu_load;
    mach_msg_type_number_t processor_msg_count;
    
    kern_return_t error = host_processor_info(mach_host_self(),
                                            PROCESSOR_CPU_LOAD_INFO,
                                            &processor_count,
                                            (processor_info_array_t *)&cpu_load,
                                            &processor_msg_count);
    
    if (error != KERN_SUCCESS) {
        printf("Error getting CPU info\n");
        return;
    }

    static processor_cpu_load_info_t previous_cpu_load = NULL;
    static mach_msg_type_number_t previous_processor_msg_count = 0;

    if (previous_cpu_load != NULL) {
        for (int i = 0; i < cpu_count; i++) {
            uint32_t user = cpu_load[i].cpu_ticks[CPU_STATE_USER] - 
                           previous_cpu_load[i].cpu_ticks[CPU_STATE_USER];
            uint32_t system = cpu_load[i].cpu_ticks[CPU_STATE_SYSTEM] - 
                             previous_cpu_load[i].cpu_ticks[CPU_STATE_SYSTEM];
            uint32_t idle = cpu_load[i].cpu_ticks[CPU_STATE_IDLE] - 
                           previous_cpu_load[i].cpu_ticks[CPU_STATE_IDLE];
            uint32_t nice = cpu_load[i].cpu_ticks[CPU_STATE_NICE] - 
                           previous_cpu_load[i].cpu_ticks[CPU_STATE_NICE];
            
            uint32_t total = user + system + idle + nice;
            cpu_usage[i] = ((float)(user + system + nice) / total) * 100.0;
        }
    }

    if (previous_cpu_load != NULL) {
        vm_deallocate(mach_task_self(), (vm_address_t)previous_cpu_load, 
                     previous_processor_msg_count);
    }
    
    previous_cpu_load = cpu_load;
    previous_processor_msg_count = processor_msg_count;
}

// Function to get GPU utilization (simplified)
float getGPUUsage() {
    io_iterator_t iterator;
    mach_port_t masterPort;
    kern_return_t kr = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if (kr != KERN_SUCCESS) return -1.0;
    
    kr = IOServiceGetMatchingServices(masterPort,
                                    IOServiceMatching("IOAccelerator"),
                                    &iterator);
    if (kr != KERN_SUCCESS) {
        return -1.0;
    }

    io_registry_entry_t regEntry = IOIteratorNext(iterator);
    IOObjectRelease(iterator);
    
    if (!regEntry) {
        return -1.0;
    }

    CFMutableDictionaryRef properties = NULL;
    kr = IORegistryEntryCreateCFProperties(regEntry,
                                         &properties,
                                         kCFAllocatorDefault,
                                         kNilOptions);
    IOObjectRelease(regEntry);

    if (kr != KERN_SUCCESS) {
        return -1.0;
    }

    if (properties) {
        CFRelease(properties);
    }
    return -1.0;
}

// Function to draw usage bar
void drawUsageBar(float percentage) {
    const int width = 30;
    int filled = (int)((percentage * width) / 100);
    
    printf("[");
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            printf("■");
        } else {
            printf(" ");
        }
    }
    printf("] %.1f%%", percentage);
}

int main() {
    // Get number of CPU cores
    int cpu_count;
    size_t size = sizeof(cpu_count);
    sysctlbyname("hw.ncpu", &cpu_count, &size, NULL, 0);
    
    float *cpu_usage = (float *)malloc(sizeof(float) * cpu_count);
    
    while (1) {
        clearScreen();
        
        // Get and display CPU usage
        getCPUUsage(cpu_usage, cpu_count);
        
        printf("\033[1m=== System Resource Monitor ===\033[0m\n\n");
        
        // Display CPU usage for each core
        for (int i = 0; i < cpu_count; i++) {
            printf("CPU Core %2d: ", i);
            drawUsageBar(cpu_usage[i]);
            printf("\n");
        }
        
        // Display GPU usage
        printf("\nGPU        : ");
        float gpu_usage = getGPUUsage();
        if (gpu_usage >= 0) {
            drawUsageBar(gpu_usage);
        } else {
            printf("Not available on this system");
        }
        
        printf("\n\nPress Ctrl+C to exit\n");
        sleep(1);
    }
    
    free(cpu_usage);
    return 0;
}
