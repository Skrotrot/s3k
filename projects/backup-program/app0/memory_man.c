/*
#include "memory_man.h"

uint64_t app1_addr;
uint64_t uart_addr;

uint32_t free_cap_idx;
uint32_t free_cap_bak_idx;
uint32_t free_cap_pmp_idx;
uint32_t free_reset_cap_index;

void dynamic_move_pmp_access(uint32_t src_pid, uint32_t dst_pid, uint32_t src_idx, uint32_t dst_idx, uint32_t dst_pmp_slot);





void dynamic_move_pmp_access(uint32_t src_pid, uint32_t dst_pid, uint32_t src_idx, uint32_t dst_idx, uint32_t dst_pmp_slot)
{
	alt_printf("DEBUG DYNAMIC 2");
	s3k_sync_mem();
	log_sys("app1 memory: pmp region unloaded from app0.", s3k_pmp_unload(src_idx));
	
	log_sys("app1 memory: region moved app0 -> app1", s3k_mon_cap_move(MONITOR, src_pid, src_idx, dst_pid, dst_idx));

	log_sys("app1 memory: pmp region loaded to app1 for access.", s3k_mon_pmp_load(MONITOR, dst_pid, dst_idx, dst_pmp_slot));
}




/*
uint32_t access_backup_mem(uint32_t region_base_address, uint32_t region_size, uint32_t src_cap_idx, uint32_t dst_pmp_slot)
{
	uint64_t app0_bak_addr = s3k_napot_encode(region_base_address, region_size);
	uint32_t tmp_cap_idx = find_free_cap();
	log_sys("app0 backup memory: region created", s3k_cap_derive(src_cap_idx, tmp_cap_idx, s3k_mk_memory(region_base_address, region_base_address + region_size, S3K_MEM_RWX)));
	free_cap_bak_idx = find_free_cap();
	log_sys("app0 backup memory: pmp cap derived", s3k_cap_derive(tmp_cap_idx, free_cap_bak_idx, s3k_mk_pmp(app0_bak_addr, S3K_MEM_RWX)));

	log_sys("app0 backup memory: region loaded to app0 for access.", s3k_pmp_load(free_cap_bak_idx, dst_pmp_slot));

	s3k_sync_mem();
	return free_cap_bak_idx;
}

*/