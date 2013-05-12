/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved                       
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: sh_read.c$
* $Version : 3.8.11.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file contains the source for an MFS shell function.
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <fio.h>
#include <mfs.h>
//#include <sh_mfs.h>
#include <shell.h>
#include <sh_prv.h>

#if SHELLCFG_USES_MFS

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_read
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_df_file(int_32 argc, char_ptr argv[])
{ /* Body */
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   uint_32           count, bytes;
   int_32            offset;
   int_32         seek_mode;
   MQX_FILE_PTR      fd = NULL;
   char_ptr          abs_path;   
   SHELL_CONTEXT_PTR    shell_ptr = Shell_get_context( argv );
   int_32               error = 0;   
      

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if ((argc < 2) || (argc > 5)) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else  {
         count = 0;
         offset = 0;
         seek_mode = IO_SEEK_CUR; 
         /* wk @130331 --> default */
//         if (argc >= 3)  {
//           if ( !Shell_parse_uint_32(argv[2], &count ))  {  // wk --> argv[2]:长度参数
//               printf("Error, invalid length\n");
//               return_code = SHELL_EXIT_ERROR;
//               print_usage=TRUE;
//            } 
//            /* check if filesystem is mounted */ 
//            else if (NULL == Shell_get_current_filesystem(argv))  
//            {
//               printf("Error, file system not mounted\n");
//               return_code = SHELL_EXIT_ERROR;
//            }            
//            else  {
//               if (argc >= 5)  {
//                  if (strcmp(argv[3], "begin") == 0) {   // WK --> 读取数据的开始位置
//                     seek_mode = IO_SEEK_SET;
//                  } else if (strcmp(argv[3], "end") == 0) { 
//                     seek_mode = IO_SEEK_END;
//                  } else if (strcmp(argv[3], "current") == 0) { 
//                     seek_mode = IO_SEEK_CUR;
//                  } else { 
//                     printf("Error, invalid seek type\n");
//                     return_code = SHELL_EXIT_ERROR;
//                     print_usage=TRUE;
//                  }
//                  
//                  if (return_code == SHELL_EXIT_SUCCESS)  {
//                     if (! Shell_parse_int_32(argv[4], &offset ))  {  // WK --> 偏移地址
//                        printf("Error, invalid seek value\n");
//                        return_code = SHELL_EXIT_ERROR;
//                        print_usage=TRUE;
//                     }
//                  }
//               }
//            }
//         }
       /* end */
         
         if (return_code == SHELL_EXIT_SUCCESS)  {
            if (MFS_alloc_path(&abs_path) != MFS_NO_ERROR) {
               printf("Error, unable to allocate memory for paths\n" );
               return_code = SHELL_EXIT_ERROR;
            } else {
               error = _io_rel2abs(abs_path,shell_ptr->CURRENT_DIR,(char *) argv[1],PATHNAME_SIZE,shell_ptr->CURRENT_DEVICE_NAME);
               if(!error)
               {
                  fd = fopen(abs_path, "w+"); 
                  printf("\nfd=%d\n",fd); // wk --> 
               }
               
               printf("\nabs_path=%s\n",abs_path); // wk --> 
             
               
               MFS_free_path(abs_path);
         /* wk @130331 --> */      
               MFS_SEARCH_DATA search_data;
               MFS_SEARCH_PARAM search;
               uint_32 error_code;
               
               MQX_FILE_PTR         fs_ptr;
                char_ptr             fs_name;
                fs_name = argv[1];
                fs_ptr = Shell_get_current_filesystem(fs_name);
                printf("fs_ptr=%s\n",fs_ptr);
                
               search.ATTRIBUTE = MFS_SEARCH_NORMAL;
               search.WILDCARD = "*.*";
               search.SEARCH_DATA_PTR = &search_data;
               error_code = ioctl(fs_ptr,IO_IOCTL_FIND_FIRST_FILE,
                                  (uint_32_ptr)&search);
               printf("\nerror_code=%d\n",error_code);
               
               while (error_code == MFS_NO_ERROR) {
                  printf ("%-12.12s  %6lu %02lu-%02lu-%04lu  %02lu:%02lu:%02lu\n"
                  , search_data.NAME, search_data.FILE_SIZE,
                  (uint_32)(search_data.DATE & MFS_MASK_MONTH) >>
                  MFS_SHIFT_MONTH,
                  (uint_32)(search_data.DATE & MFS_MASK_DAY)   >> 
                  MFS_SHIFT_DAY,
                  (uint_32)((search_data.DATE & MFS_MASK_YEAR) >> 
                  MFS_SHIFT_YEAR) + 1980,
                  (uint_32)(search_data.TIME & MFS_MASK_HOURS)   >> 
                  MFS_SHIFT_HOURS,
                  (uint_32)(search_data.TIME & MFS_MASK_MINUTES) >> 
                  MFS_SHIFT_MINUTES,
                  (uint_32)(search_data.TIME & MFS_MASK_SECONDS) << 1);
                  
                error_code = ioctl(fs_ptr, IO_IOCTL_FIND_NEXT_FILE, 
                                  (uint_32_ptr) &search_data);
               }
               
               /* get long file name */
//                MFS_GET_LFN_STRUCT   lfn_struct;
//                char                 lfn[FILENAME_SIZE + 1];
////                char                 filepath = “\\temp\longfi~1.txt”; 
//                uint_32              error_code;
//   
//                lfn_struct.PATHNAME = abs_path;
//                lfn_struct.LONG_FILENAME = lfn;
//                MQX_FILE_PTR         fs_ptr;
//                char_ptr             fs_name;
//                fs_name = argv[2];
//                fs_ptr = Shell_get_current_filesystem(fs_name);
//                printf("fs_ptr=%s\n",fs_ptr);
//                 
//                printf("\nfilename=%s\n",argv[1]);
//                
//                error_code = ioctl(fs_ptr, IO_IOCTL_GET_LFN, 
//                     (uint_32_ptr) &lfn_struct);
//                
//                printf("\nerror_code=%x\n",error_code);
//                if (!error_code)  {
//                    printf("%s\n", lfn);      
//                }
                /* end */
          /* wk @130331 --> end */            
//               if (fd && !error)  {
//                  bytes = 0;
//                  if (fseek(fd, offset, seek_mode) != IO_ERROR)  {
//                     printf("Reading from %s:\n", argv[1]);
//                     while (!feof(fd)) {
//                        fputc(fgetc(fd), stdout);
//                        if (++bytes == count) break;
//                     } 
//                     printf("\nDone.\n");
//                     fclose(fd);
//                  } else  {
//                     printf("Error, unable to seek file %s.\n", argv[1] );
//                     return_code = SHELL_EXIT_ERROR;
//                  }
//               } else  {
//                  printf("Error, unable to open file %s.\n", argv[1] );
//                  return_code = SHELL_EXIT_ERROR;
//               }
               
            }
         }
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <filename> <bytes> [<seek_mode>] [<offset>]\n", argv[0]);
      } else  {
         printf("Usage: %s <filename> <bytes> [<seek_mode>] [<offset>]\n", argv[0]);
         printf("   <filename>   = filename to display\n");
         printf("   <bytes>      = number of bytes to read\n");
         printf("   <seek_mode>  = one of: begin, end or current\n");
         printf("   <offset>     = seek offset\n");
      }
   }
   return return_code;
} /* Endbody */

#endif //SHELLCFG_USES_MFS

/* EOF*/
