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
* $FileName: sh_write.c$
* $Version : 3.8.14.0$
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
#include <shell.h>
#include <sh_prv.h>

#if SHELLCFG_USES_MFS
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_write_binary
* Author           :  WK 
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*               以二进制文件方式写
*
*END*---------------------------------------------------------------------*/

int_32  Shell_write_binary(int_32 argc, char_ptr argv[] ,uchar num,uchar_ptr data)
{ /* Body */
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   uint_32           count=0, i;
   int_32            offset=0;
   int_32            seek_mode=0;
   char              c;
   MQX_FILE_PTR      fd = NULL;
   char_ptr          abs_path;   
   boolean           cache_enabled=TRUE;
   SHELL_CONTEXT_PTR    shell_ptr = Shell_get_context( argv );
   char                 buf[SHELL_BLOCK_SIZE];
   char *               argv5_p;
   int_32               error = 0;
   _mqx_int             bi;   

   print_usage = Shell_check_help_request(argc, argv, &shorthelp ); // wk @130405 --> 简化程序减小时间

   if (!print_usage)  {
      if ((argc < 3)) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } 
      /* check if filesystem is mounted */ 
      else if (NULL == Shell_get_current_filesystem(argv))  
      {
         printf("Error, file system not mounted\n");
         return_code = SHELL_EXIT_ERROR;
      }
      else  {
         count = 0;
         offset = 0;
         seek_mode = IO_SEEK_CUR; // 默认模式
         if (argc >= 4)  {
           
//            if (! Shell_parse_uint_32(argv[2], &count ))  {// 检查输入长度
//               printf("Error, invalid length\n");
//               return_code = SHELL_EXIT_ERROR;
//               print_usage=TRUE;
//            } else  
           
            {
               if (argc >= 4)  {
                  if (strcmp(argv[2], "begin") == 0) {
                     seek_mode = IO_SEEK_SET;
                  } else if (strcmp(argv[2], "end") == 0) { 
                     seek_mode = IO_SEEK_END;
                  } else if (strcmp(argv[2], "current") == 0) { 
                     seek_mode = IO_SEEK_CUR;
                  } else { 
                     printf("Error, invalid seek type\n");
                     return_code = SHELL_EXIT_ERROR;
                     print_usage=TRUE;
                  }
                  
                  if (return_code == SHELL_EXIT_SUCCESS)  {
                     if (! Shell_parse_int_32(argv[3], &offset ))  {
                        printf("Error, invalid seek value\n");
                        return_code = SHELL_EXIT_ERROR;
                        print_usage=TRUE;
                     }
                  }
               }
            }
         }

      }
     
      if (return_code == SHELL_EXIT_SUCCESS)  {
         if (MFS_alloc_path(&abs_path) != MFS_NO_ERROR) {
            printf("Error, unable to allocate memory for paths\n" );
            return_code = SHELL_EXIT_ERROR;
         } else {
             error = _io_rel2abs(abs_path,shell_ptr->CURRENT_DIR,(char *) argv[1],PATHNAME_SIZE,shell_ptr->CURRENT_DEVICE_NAME);
             if(!error)
             {
                fd = fopen(abs_path, "a");
             }
             if (fd && !error)  {
                if (fseek(fd, offset, seek_mode ) != IO_ERROR)  {
               
               // generate data to buf
//               argv5_p = (char*)argv[5];// by dx
//               for (i = 0, c = '0', bi = 0; i < count;i++) {// 写入txt，从字符0到z
//                  
//                  buf[bi++] = c;
//                  
//                  if (bi > SHELL_BLOCK_SIZE - 1) {
//                     write(fd, buf, SHELL_BLOCK_SIZE);
//                     bi = 0;
//                  }
//                  
//                  if (c == 'z')
//                     c='0';
//                  else
//                     c++;
//               }
//               
//               if (bi)                  
//                  count = ( strlen(argv5_p)<count )?( strlen(argv5_p) ):(count);// 修改长度，若超过最大长度则用最大长度
//                  write(fd, argv5_p, count);// 写入SD，相应路径
  
                  /* wk @130405 --> 写我二进制数据 */
//                  for(i = 0; i < num; i++)
//                  {
////                    write(fd,data+i,1);
//                  }
                   write(fd,data,num);
                   /* wk @130405 --> 写我二进制数据 <--END */
                  
                  fclose(fd);
               } else  {
                  printf("Error, unable to seek file %s.\n", argv[1] );
                  return_code = SHELL_EXIT_ERROR;
               }
            } else  {
                printf("Error, unable to open file %s.\n", argv[1] );
                return_code = SHELL_EXIT_ERROR;
            }
            MFS_free_path(abs_path);
         }
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <filename> <bytes> <seek_mode> <offset> <string>\n", argv[0]);
      } else  {
         printf("Usage: %s <filename> <bytes> <seek_mode> <offset> <string>\n", argv[0]);
         printf("   <filename>   = filename to write\n");
         printf("   <bytes>      = number of bytes to write\n");
         printf("   <seek_mode>  = one of: begin, end or current\n");
         printf("   <offset>     = seek offset\n");
         printf("   <string>     = string\n");
      }
   }
   return return_code;
} /* Endbody */

#endif //SHELLCFG_USES_MFS
/* EOF */
