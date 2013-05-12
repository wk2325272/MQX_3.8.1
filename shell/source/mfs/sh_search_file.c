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
* $FileName: Shell_search_file.c$
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
* Function Name    :   Shell_search_file
* Returned Value   :  int_32 error code
* Comments  :  search files  .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_search_file(int_32 argc, char_ptr argv[])
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
      if ((argc < 2) || (argc > 3)) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else  {
         count = 0;
         offset = 0;
         seek_mode = IO_SEEK_CUR; 
         
         if (return_code == SHELL_EXIT_SUCCESS)  {
            if (MFS_alloc_path(&abs_path) != MFS_NO_ERROR) {
               printf("Error, unable to allocate memory for paths\n" );
               return_code = SHELL_EXIT_ERROR;
            } else {
               error = _io_rel2abs(abs_path,shell_ptr->CURRENT_DIR,(char *) argv[1],PATHNAME_SIZE,shell_ptr->CURRENT_DEVICE_NAME);
               if(!error)
               {
                  fd = fopen(abs_path, "w+"); 
               }        
               MFS_free_path(abs_path);
               /* wk @130331 --> search files */      
               MFS_SEARCH_DATA search_data;
               MFS_SEARCH_PARAM search;
               MQX_FILE_PTR         fs_ptr;
               char_ptr             fs_name;
               uint_32 error_code;
               char_ptr             file_name,source_name; 
               
               fs_name = argv[1];
               search.ATTRIBUTE = MFS_SEARCH_NORMAL;
               search.WILDCARD = "*.*";
               search.SEARCH_DATA_PTR = &search_data;
               
//               fs_ptr = Shell_get_current_filesystem(fs_name);  // wk @130401--> 获取驱动设备的指针
//              fs_ptr=_io_get_fs_by_name(abs_path);
                fs_ptr = _io_get_fs_by_name(argv[1]);
               error_code = ioctl(fs_ptr,IO_IOCTL_FIND_FIRST_FILE,
                                  (uint_32_ptr)&search);
               printf("\nerror_code=%d\n",error_code);
//               file_name=search_data.NAME;
               while (error_code == MFS_NO_ERROR) {
                 
                  file_name=search_data.NAME;
                  source_name=argv[2];
                  printf ("%-12.12s  %6lu \n"
                    , search_data.NAME, search_data.FILE_SIZE);
                   
                  if(argc==3) 
                  {
                    while(*(source_name)!='\0')
                     if(*(file_name++)!= *(source_name++))  // wk--> 如果有3个参数，则第三个参数是搜索文件的名称，但是注意：字母都必须大些
                      goto over;
                  
                    break;  // wk--> 找到了想要的文件，就跳出循环
                  }
                    
 over:              error_code = ioctl(fs_ptr, IO_IOCTL_FIND_NEXT_FILE, // wk--> 没有找到了想要的文件，接着找
                                    (uint_32_ptr) &search_data);
          
               }
               
            }
         }
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <dir> <filename>\n", argv[0]);
      } else  {
         printf("Usage: %s <dir> <filename>\n", argv[0]);
         printf("   <dir>   = dir to search\n");
         printf("   <filename>      = filename to search\n");
      }
   }
   return return_code;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_search_file_1
* Returned Value   :  int_32 error code
* Comments  :  the func of searching files for calling in the aplication of users  .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_search_file_1(int_32 argc, char_ptr argv[], uint_32 size,uint_16 status)
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
      if ((argc < 2) || (argc > 3)) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else  {
         count = 0;
         offset = 0;
         seek_mode = IO_SEEK_CUR; 
         
         if (return_code == SHELL_EXIT_SUCCESS)  {
            if (MFS_alloc_path(&abs_path) != MFS_NO_ERROR) {
               printf("Error, unable to allocate memory for paths\n" );
               return_code = SHELL_EXIT_ERROR;
            } else {
               error = _io_rel2abs(abs_path,shell_ptr->CURRENT_DIR,(char *) argv[1],PATHNAME_SIZE,shell_ptr->CURRENT_DEVICE_NAME);
               if(!error)
               {
                  fd = fopen(abs_path, "w+"); 
               }        
               MFS_free_path(abs_path);
               /* wk @130331 --> search files */      
               MFS_SEARCH_DATA search_data;
               MFS_SEARCH_PARAM search;
               MQX_FILE_PTR         fs_ptr;
               char_ptr             fs_name;
               uint_32 error_code;
               char_ptr             file_name,source_name; 
               
               fs_name = argv[1];
               search.ATTRIBUTE = MFS_SEARCH_NORMAL;
               search.WILDCARD = "*.*";
               search.SEARCH_DATA_PTR = &search_data;
               
               fs_ptr = Shell_get_current_filesystem(fs_name);  // wk @130401--> 获取驱动设备的指针
               error_code = ioctl(fs_ptr,IO_IOCTL_FIND_FIRST_FILE,
                                  (uint_32_ptr)&search);
//               printf("\nerror_code=%d\n",error_code);
               status=error_code;
              
               while (error_code == MFS_NO_ERROR) {
                 
                  file_name=search_data.NAME;
                  source_name=argv[2];
                  
                  printf ("%-12.12s  %6lu \n"
                    , search_data.NAME, search_data.FILE_SIZE);
                   
                  if(argc==3) 
                  {
                    while(*(source_name)!='\0')
                     if(*(file_name++)!= *(source_name++))  // wk--> 如果有3个参数，则第三个参数是搜索文件的名称，但是注意：字母都必须大些
                      goto over;
                  
                    break;  // wk--> 找到了想要的文件，就跳出循环
                  }
                    
 over:               error_code = ioctl(fs_ptr, IO_IOCTL_FIND_NEXT_FILE, 
                                    (uint_32_ptr) &search_data);
          
               }

               size=search_data.FILE_SIZE;  // wk --> 获得想要查找文件长度或是最后一个文件长度
               
            }
         }
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <dir> <filename>\n", argv[0]);
      } else  {
         printf("Usage: %s <dir> <filename>\n", argv[0]);
         printf("   <dir>   = dir to search\n");
         printf("   <filename>      = filename to search\n");
      }
   }
   return return_code;
} /* Endbody */
#endif //SHELLCFG_USES_MFS

/* EOF*/
