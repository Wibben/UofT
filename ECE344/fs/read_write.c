#include "testfs.h"
#include "list.h"
#include "super.h"
#include "block.h"
#include "inode.h"

/* given logical block number, read the corresponding physical block into block.
 * return physical block number.
 * returns 0 if physical block does not exist.
 * returns negative value on other errors. */
static int
testfs_read_block(struct inode *in, int log_block_nr, char *block)
{
    // Block number out of bounds
    if(log_block_nr>=(NR_DIRECT_BLOCKS+NR_INDIRECT_BLOCKS+(NR_INDIRECT_BLOCKS*NR_INDIRECT_BLOCKS))) return -EFBIG;
    
    int phy_block_nr = 0;

    assert(log_block_nr >= 0);   
    if (log_block_nr < NR_DIRECT_BLOCKS) {
        phy_block_nr = (int)in->in.i_block_nr[log_block_nr];
    } else {
        log_block_nr -= NR_DIRECT_BLOCKS;

        if (log_block_nr >= NR_INDIRECT_BLOCKS && in->in.i_dindirect>0) {
            log_block_nr -= NR_INDIRECT_BLOCKS;
            
            read_blocks(in->sb, block, in->in.i_dindirect, 1);
            
            int offset = log_block_nr/NR_INDIRECT_BLOCKS;
            if(((int*)block)[offset]>0) {
                read_blocks(in->sb, block, ((int*)block)[offset], 1);

                // Update physical block number
                phy_block_nr = ((int*)block)[log_block_nr%NR_INDIRECT_BLOCKS];
            } else return 0;
        } else if (in->in.i_indirect > 0) {
            read_blocks(in->sb, block, in->in.i_indirect, 1);
            phy_block_nr = ((int *)block)[log_block_nr];
        }
    }
    if (phy_block_nr > 0) {
        read_blocks(in->sb, block, phy_block_nr, 1);
    } else {
        /* we support sparse files by zeroing out a block that is not
         * allocated on disk. */
        bzero(block, BLOCK_SIZE);
    }
    return phy_block_nr;
}

int
testfs_read_data(struct inode *in, char *buf, off_t start, size_t size)
{
    char block[BLOCK_SIZE];
    long block_nr = start / BLOCK_SIZE; // logical block number in the file
    long block_ix = start % BLOCK_SIZE; //  index or offset in the block
    int ret;
    int bytesRead = 0;

    assert(buf);
    if (start + (off_t) size > in->in.i_size) {
        size = in->in.i_size - start;
    }
    
    ret = testfs_read_block(in, block_nr, block);
    // Copy block to buffer
    while(block_ix + (int)size > BLOCK_SIZE + bytesRead && ret>=0) {
        memcpy(buf + bytesRead,block + block_ix, BLOCK_SIZE-block_ix);
        bytesRead += BLOCK_SIZE-block_ix;
        block_ix = 0;
        block_nr++;
        // Re-evaluate ret
        ret = testfs_read_block(in, block_nr, block);
    }
    if (ret < 0) return ret;
    // Copy over leftover block
    memcpy(buf+bytesRead, block + block_ix, size-bytesRead);
    bytesRead += size-bytesRead;
    /* return the number of bytes read or any error */
    return bytesRead;
}

/* given logical block number, allocate a new physical block, if it does not
 * exist already, and return the physical block number that is allocated.
 * returns negative value on error. */
static int
testfs_allocate_block(struct inode *in, int log_block_nr, char *block)
{
    // Block number out of bounds
    if(log_block_nr>= NR_DIRECT_BLOCKS+NR_INDIRECT_BLOCKS+(NR_INDIRECT_BLOCKS*NR_INDIRECT_BLOCKS)) return -EFBIG;
    
    int phy_block_nr;
    char indirect[BLOCK_SIZE];
    int indirect_allocated = 0;

    assert(log_block_nr >= 0);
    phy_block_nr = testfs_read_block(in, log_block_nr, block);

    /* phy_block_nr > 0: block exists, so we don't need to allocate it, 
       phy_block_nr < 0: some error */
    if (phy_block_nr != 0)
        return phy_block_nr;

    /* allocate a direct block */
    if (log_block_nr < NR_DIRECT_BLOCKS) {
        assert(in->in.i_block_nr[log_block_nr] == 0);
        phy_block_nr = testfs_alloc_block_for_inode(in);
        if (phy_block_nr >= 0) {
            in->in.i_block_nr[log_block_nr] = phy_block_nr;
        }
        return phy_block_nr;
    }

    log_block_nr -= NR_DIRECT_BLOCKS;
    if (log_block_nr >= NR_INDIRECT_BLOCKS) {
        log_block_nr -= NR_INDIRECT_BLOCKS;
        
        // Create a doubly indirect block
        char dindirect[BLOCK_SIZE];
        int dindirect_allocated = 0;
        int offset = log_block_nr/NR_INDIRECT_BLOCKS;
        
        // Allocate doubly indirect block if required
        if(in->in.i_dindirect == 0){
            bzero(dindirect, BLOCK_SIZE);
            phy_block_nr = testfs_alloc_block_for_inode(in);
            
            if(phy_block_nr < 0) return phy_block_nr;
            
            in->in.i_dindirect = phy_block_nr;
            dindirect_allocated = 1;
        } else{ // Not required to allocate
            read_blocks(in->sb, dindirect, in->in.i_dindirect, 1);
        }
        
        // Allocate indirect block if required
        if(((int*) dindirect)[offset] == 0) {
            bzero(indirect, BLOCK_SIZE);
            phy_block_nr = testfs_alloc_block_for_inode(in);
            if (phy_block_nr < 0){
                if (dindirect_allocated) testfs_free_block_from_inode(in, in->in.i_dindirect);
                
                return phy_block_nr;
            }
            
            ((int*)dindirect)[offset] = phy_block_nr;
            write_blocks(in->sb, dindirect, in->in.i_dindirect, 1);
            indirect_allocated = 1;
        }else{ // Not required to allocate
            read_blocks(in->sb, indirect, ((int*)dindirect)[offset], 1);
        }
        
        // Allocate direct block if required
        phy_block_nr = testfs_alloc_block_for_inode(in);
        
        if(phy_block_nr < 0){
            if(indirect_allocated) testfs_free_block_from_inode(in, ((int *)dindirect)[offset]);
            if(dindirect_allocated) testfs_free_block_from_inode(in, in->in.i_dindirect);
            
            return phy_block_nr;
        } 
        
        ((int*)indirect)[log_block_nr%NR_INDIRECT_BLOCKS] = phy_block_nr;
        write_blocks(in->sb, indirect, ((int *)dindirect)[offset], 1);
         
        return phy_block_nr;
    }

    if (in->in.i_indirect == 0) {	/* allocate an indirect block */
        bzero(indirect, BLOCK_SIZE);
        phy_block_nr = testfs_alloc_block_for_inode(in);
        if (phy_block_nr < 0)
            return phy_block_nr;
        indirect_allocated = 1;
        in->in.i_indirect = phy_block_nr;
    } else {	/* read indirect block */
        read_blocks(in->sb, indirect, in->in.i_indirect, 1);
    }

    /* allocate direct block */
    assert(((int *)indirect)[log_block_nr] == 0);	
    phy_block_nr = testfs_alloc_block_for_inode(in);

    if (phy_block_nr >= 0) {
        /* update indirect block */
        ((int *)indirect)[log_block_nr] = phy_block_nr;
        write_blocks(in->sb, indirect, in->in.i_indirect, 1);
    } else if (indirect_allocated) {
        /* there was an error while allocating the direct block, 
         * free the indirect block that was previously allocated */
        testfs_free_block_from_inode(in, in->in.i_indirect);
        in->in.i_indirect = 0;
    }
    return phy_block_nr;
}

int
testfs_write_data(struct inode *in, const char *buf, off_t start, size_t size)
{
    char block[BLOCK_SIZE];
    long block_nr = start / BLOCK_SIZE; // logical block number in the file
    long block_ix = start % BLOCK_SIZE; //  index or offset in the block
    int ret;
    int bytesWrote = 0;

    /* ret is the newly allocated physical block number */
    ret = testfs_allocate_block(in, block_nr, block);
    if(ret<0) {
        if(ret == -EFBIG) in->in.i_size = MAX(in->in.i_size, start + (off_t)bytesWrote);
        return ret;
    }
    // Same thing as read, but copy buffer to block
    while(block_ix+(int)size > BLOCK_SIZE+bytesWrote && ret>=0) {
        memcpy(block+block_ix, buf+bytesWrote, BLOCK_SIZE-block_ix);
        write_blocks(in->sb, block, ret, 1);
        bytesWrote += BLOCK_SIZE-block_ix;
        block_ix = 0;
        block_nr++;
        // Re-evaluate ret
        ret = testfs_allocate_block(in, block_nr, block);
    }
    
    if(ret<0) {
        in->in.i_size = MAX(in->in.i_size, start + (off_t)bytesWrote);
        return ret;
    }
    
    // Copy over rest of block
    memcpy(block + block_ix, buf+bytesWrote, size-bytesWrote);
    write_blocks(in->sb, block, ret, 1);
    /* increment i_size by the number of bytes written. */
    if (size > 0)
        in->in.i_size = MAX(in->in.i_size, start + (off_t) size);
    in->i_flags |= I_FLAGS_DIRTY;
    /* return the number of bytes written or any error */
    return size;
}

int
testfs_free_blocks(struct inode *in)
{
    int i;
    int e_block_nr;

    /* last logical block number */
    e_block_nr = DIVROUNDUP(in->in.i_size, BLOCK_SIZE);

    /* remove direct blocks */
    for (i = 0; i < e_block_nr && i < NR_DIRECT_BLOCKS; i++) {
        if (in->in.i_block_nr[i] == 0)
            continue;
        testfs_free_block_from_inode(in, in->in.i_block_nr[i]);
        in->in.i_block_nr[i] = 0;
    }
    e_block_nr -= NR_DIRECT_BLOCKS;

    /* remove indirect blocks */
    if (in->in.i_indirect > 0) {
        char block[BLOCK_SIZE];
        assert(e_block_nr > 0);
        read_blocks(in->sb, block, in->in.i_indirect, 1);
        for (i = 0; i < e_block_nr && i < NR_INDIRECT_BLOCKS; i++) {
            if (((int *)block)[i] == 0)
                continue;
            testfs_free_block_from_inode(in, ((int *)block)[i]);
            ((int *)block)[i] = 0;
        }
        testfs_free_block_from_inode(in, in->in.i_indirect);
        in->in.i_indirect = 0;
    }

    e_block_nr -= NR_INDIRECT_BLOCKS;
    /* handle double indirect blocks */
    if (e_block_nr >= 0) {
        if(in->in.i_dindirect>0) {
            char block[BLOCK_SIZE];
            read_blocks(in->sb, block, in->in.i_dindirect, 1);

            for(int i=0; i<NR_INDIRECT_BLOCKS && i<=(e_block_nr/NR_INDIRECT_BLOCKS); i++) {
                if(((int*)block)[i]>0) {
                    char indirect[BLOCK_SIZE];
                    read_blocks(in->sb, indirect, ((int*)block)[i], 1);
                    for(int j=0; j<NR_INDIRECT_BLOCKS && j<e_block_nr-(NR_INDIRECT_BLOCKS*i); j++) {
                        if(((int*)indirect)[j]>0) {
                            testfs_free_block_from_inode(in,((int*)indirect)[j]);
                            ((int*)indirect)[j] = 0;
                        }
                    }
                    testfs_free_block_from_inode(in,((int*)block)[i]);
                    ((int*)block)[i] = 0;
                }
            }
            testfs_free_block_from_inode(in,in->in.i_dindirect);
            in->in.i_indirect = 0;
        }
    }
    
    in->in.i_size = 0;
    in->i_flags |= I_FLAGS_DIRTY;
    return 0;
}
