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
* $FileName: sh_dir.c$
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
#define BUFFER_SIZE  256

#define  FS_DIR_MODE_UNIX        1
#define  FS_DIR_MODE_MSDOS       2
//#define  FS_DIR_MODE_MSDOS       0 // wk -->
#define  FS_DIR_MODE_FILENAME    3

typedef struct
{
    MQX_FILE_PTR         FS_PTR;
    MFS_GET_LFN_STRUCT   LFN_DATA;
    MFS_SEARCH_DATA      SEARCH_DATA;
    MFS_SEARCH_PARAM     SEARCH;
    char                 LFN[PATHNAME_SIZE];
    uint_32              MODE;
    boolean              FIRST;
} DIR_STRUCT, _PTR_ DIR_STRUCT_PTR;

//DIR_STRUCT_PTR       dir_ptr_g;
   
//int_32 _io_mfs_dir_read_wk( pointer dir, char_ptr buffer, uint_32 size);
int_32 _io_mfs_dir_read_wk1( pointer dir, char_ptr buffer, uint_32 size,DIR_STRUCT_PTR dir_ptr_glo);
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_search_file_r
* Returned Value   :  int_32 error code
* Comments  :  find files .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_search_file_r(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean              print_usage, shorthelp = FALSE;
   int_32               return_code = SHELL_EXIT_SUCCESS;
   int_32               len;
   MQX_FILE_PTR         fs_ptr;
   char_ptr             path_ptr, mode_ptr;
   pointer              dir_ptr;
   char_ptr             buffer = NULL;
   SHELL_CONTEXT_PTR    shell_ptr = Shell_get_context( argv );
   int_32               error = 0;
   char_ptr             file_name,source_name; 
   MFS_SEARCH_DATA search_data;
   pointer      search_ptr;
   DIR_STRUCT_PTR dir_file; // wk @130405 -->
   dir_file = _mem_alloc_zero( sizeof( DIR_STRUCT ));  // wk @130405 -->
   
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc > 2)  {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else {
         path_ptr  ="*.*";
         mode_ptr = "m";

         source_name=argv[1];
         
         fs_ptr = Shell_get_current_filesystem(argv);
         /* check if filesystem is mounted */ 
         if (fs_ptr == NULL)  {
             printf("Error, file system not mounted\n");
             return_code = SHELL_EXIT_ERROR;
         } else  {
            buffer = _mem_alloc(BUFFER_SIZE);
            error = ioctl(fs_ptr, IO_IOCTL_CHANGE_CURRENT_DIR, shell_ptr->CURRENT_DIR);
            if (buffer && !error) {
            
               dir_ptr = _io_mfs_dir_open(fs_ptr, path_ptr, mode_ptr );
            
               if (dir_ptr == NULL)  {
                  printf("File not found.\n");
                  return_code = SHELL_EXIT_ERROR;
               } else {
                 
                 
//                  while ((_io_is_fs_valid(fs_ptr)) && (len = _io_mfs_dir_read_wk(dir_ptr, buffer, BUFFER_SIZE )) > 0) {  // wk @130405-->old
                  while ((_io_is_fs_valid(fs_ptr)) && (len = _io_mfs_dir_read_wk1(dir_ptr, buffer, BUFFER_SIZE,dir_file )) > 0) {
//                    printf ("%-12.12s  %6lu \n"
//                    ,dir_ptr_g->SEARCH_DATA.NAME, dir_ptr_g->SEARCH_DATA.FILE_SIZE);
//                    file_name=dir_ptr_g->SEARCH_DATA.NAME;   // wk @130405-->old                   
                    printf ("%-12.12s  %6lu \n"
                    ,dir_file->SEARCH_DATA.NAME, dir_file->SEARCH_DATA.FILE_SIZE);
//                    file_name=dir_ptr_g->SEARCH_DATA.NAME; // wk @130405-->old
                    file_name=dir_file->SEARCH_DATA.NAME;
                    if(argc==2)
                    {
                      while(*(source_name)!='\0')
                       if(*(file_name++)!= *(source_name++))
                        goto next;
                      
                       error=33;   // WK --> 文件找到
                       break;     
                    }
 next:                 source_name=argv[1];                     
                  } 
                  if(argc==2)
                  printf("error=%d if /t33表示文件找到\n",error);
                  _io_mfs_dir_close(dir_ptr);
               }
               
               _mem_free(buffer);
               _mem_free(dir_file);
            } else {
               if(buffer == NULL){
                 printf("Error, unable to allocate space.\n" );
               } else {
                 printf("Error, directory does not exist.\n" );
               }
               return_code = SHELL_EXIT_ERROR;
            }
         }
      }
   }

   if (print_usage)  {
      if (shorthelp)  {
         printf("%s [<filename>]\n", argv[0]);
      } else  {
         printf("Usage: %s [<filespec> [<attr>]]\n", argv[0]);
         printf("   <filename>   = filename what want to fine\n");
      }
   }
   return return_code;
} /* Endbody */
#endif //SHELLCFG_USES_MFS
/* EOF*/
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_search_file_r1
* Returned Value   :  status --> 文件是否找到，找到了为1 没有找到为0
* Comments  :  find files for api calls for the users .
*
*END*---------------------------------------------------------------------*/
uchar  Shell_search_file_r1(int_32 argc, char_ptr argv[],uint_32_ptr size)
{ /* Body */
   boolean              print_usage, shorthelp = FALSE;
   int_32               return_code = SHELL_EXIT_SUCCESS;
   int_32               len;
   MQX_FILE_PTR         fs_ptr;
   char_ptr             path_ptr, mode_ptr;
   pointer              dir_ptr;
   char_ptr             buffer = NULL;
   SHELL_CONTEXT_PTR    shell_ptr = Shell_get_context( argv );
   int_32               error = 0;
   char_ptr             file_name,source_name; 
   uchar status=0;  // wk --> 初始值，表示文件未找到
   DIR_STRUCT_PTR dir_file; // wk @130405 -->
   dir_file = _mem_alloc_zero( sizeof( DIR_STRUCT ));  // wk @130405 -->

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc > 2)  {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else {
         path_ptr  ="*.*";
         mode_ptr = "m";

         source_name=argv[1];
         
         fs_ptr = Shell_get_current_filesystem(argv);
         /* check if filesystem is mounted */ 
         if (fs_ptr == NULL)  {
             printf("Error, file system not mounted\n");
             return_code = SHELL_EXIT_ERROR;
         } else  {
            buffer = _mem_alloc(BUFFER_SIZE);
            error = ioctl(fs_ptr, IO_IOCTL_CHANGE_CURRENT_DIR, shell_ptr->CURRENT_DIR);
            if (buffer && !error) {
            
               dir_ptr = _io_mfs_dir_open(fs_ptr, path_ptr, mode_ptr );
            
               if (dir_ptr == NULL)  {
                  printf("File not found.\n");
                  return_code = SHELL_EXIT_ERROR;
               } else {
                                 
//                  while ((_io_is_fs_valid(fs_ptr)) && (len = _io_mfs_dir_read_wk(dir_ptr, buffer, BUFFER_SIZE )) > 0) {
                 /* wk @130405 --> 通过参数传回查找文件的属性 */
                  while ((_io_is_fs_valid(fs_ptr)) && (len = _io_mfs_dir_read_wk1(dir_ptr, buffer, BUFFER_SIZE,dir_file )) > 0) {
                    
//                    printf ("%-12.12s  %6lu \n"
//                    ,dir_ptr_g->SEARCH_DATA.NAME, dir_ptr_g->SEARCH_DATA.FILE_SIZE);
//                    file_name=dir_ptr_g->SEARCH_DATA.NAME;
                    
                    printf ("%-12.12s  %6lu \n"
                            ,dir_file->SEARCH_DATA.NAME, dir_file->SEARCH_DATA.FILE_SIZE);
                    file_name=dir_file->SEARCH_DATA.NAME;
                    
                    if(argc==2)
                    {
                      while(*(source_name)!='\0')
                       if(*(file_name++)!= *(source_name++))
                        goto next;
                      
                       status=1;   // WK --> 文件找到了
                       error=33;
                       *size = dir_file->SEARCH_DATA.FILE_SIZE ;
                       break;
                       
                    }
 next:                 source_name=argv[1];                     
                  } 
                  if(argc==2)
//                  printf("error=%d if /t33表示文件找到\n",error);
                  _io_mfs_dir_close(dir_ptr);
               }
               
               _mem_free(buffer);
               _mem_free(dir_file);
            } else {
               if(buffer == NULL){
                 printf("Error, unable to allocate space.\n" );
               } else {
                 printf("Error, directory does not exist.\n" );
               }
               return_code = SHELL_EXIT_ERROR;
            }
         }
      }
   }

   if (print_usage)  {
      if (shorthelp)  {
         printf("%s [<filename>]\n", argv[0]);
      } else  {
         printf("Usage: %s [<filespec> [<attr>]]\n", argv[0]);
         printf("   <filename>   = filename what want to fine\n");
      }
   }
   
//   return return_code;
    return status;
} /* Endbody */
/* EOF*/

/*******************************************************************************
** Function Name	：_io_mfs_dir_read_wk
** Input		：
** Return		：
** Author		：
** Version	：
** Date		：
** Dessription	： 查找 --> 用的是全局变量传回参数，不是最佳选择
** Reverse	：
*******************************************************************************/

//int_32 _io_mfs_dir_read_wk( pointer dir, char_ptr buffer, uint_32 size)
//{
//    dir_ptr_g = (DIR_STRUCT_PTR) dir;
//    uint_32              error_code;
//    uint_32              len=0, month, year;
//
//    if ( dir_ptr_g == NULL ) return IO_ERROR;
//    
//    if ( dir_ptr_g->FIRST )
//    {
//        error_code = ioctl(dir_ptr_g->FS_PTR, IO_IOCTL_FIND_FIRST_FILE, (pointer)&dir_ptr_g->SEARCH);
//        dir_ptr_g->FIRST = FALSE;
//    }
//    else
//    {
//        error_code = ioctl(dir_ptr_g->FS_PTR, IO_IOCTL_FIND_NEXT_FILE,  (pointer) &dir_ptr_g->SEARCH_DATA);
//    }
//    if ( error_code != MFS_NO_ERROR )
//    {
//        return IO_ERROR;
//    }
//    
////    printf("error_code=%x\n",error_code); // wk --> test
//    
//    /* read file */
//    error_code = ioctl(dir_ptr_g->FS_PTR, IO_IOCTL_GET_LFN, (pointer)&dir_ptr_g->LFN_DATA);
//    /* attributes test */
//        
//    if ( !error_code )
//    {
//        len += sprintf (&buffer[len], "%s\r\n",dir_ptr_g->LFN ); 
//    }
//    else
//    {
//        len += sprintf (&buffer[len], "%s\r\n",dir_ptr_g->SEARCH_DATA.NAME ); 
//    }
//    return len;
//} 

/*******************************************************************************
** Function Name	：_io_mfs_dir_read_wk
** Input		：
** Return		：
** Author		：
** Version	：
** Date		：
** Dessription	： 查找 -->
** Reverse	：
*******************************************************************************/
int_32 _io_mfs_dir_read_wk1( pointer dir, char_ptr buffer, uint_32 size,DIR_STRUCT_PTR dir_ptr_glo)
{
    DIR_STRUCT_PTR dir_ptr_gl = (DIR_STRUCT_PTR) dir;
    uint_32              error_code;
    uint_32              len=0, month, year;

    if ( dir_ptr_gl == NULL ) return IO_ERROR;
    
    if ( dir_ptr_gl->FIRST )
    {
        error_code = ioctl(dir_ptr_gl->FS_PTR, IO_IOCTL_FIND_FIRST_FILE, (pointer)&dir_ptr_gl->SEARCH);
        dir_ptr_gl->FIRST = FALSE;
    }
    else
    {
        error_code = ioctl(dir_ptr_gl->FS_PTR, IO_IOCTL_FIND_NEXT_FILE,  (pointer) &dir_ptr_gl->SEARCH_DATA);
    }
    if ( error_code != MFS_NO_ERROR )
    {
        return IO_ERROR;
    }
    
//    printf("error_code=%x\n",error_code); // wk --> test
    
    /* read file */
    error_code = ioctl(dir_ptr_gl->FS_PTR, IO_IOCTL_GET_LFN, (pointer)&dir_ptr_gl->LFN_DATA);
    /* attributes test */
        
    if ( !error_code )
    {
        len += sprintf (&buffer[len], "%s\r\n",dir_ptr_gl->LFN ); 
    }
    else
    {
        len += sprintf (&buffer[len], "%s\r\n",dir_ptr_gl->SEARCH_DATA.NAME ); 
    }
    
    *dir_ptr_glo=*dir_ptr_gl;  //wk @130405 --> 查找文件参数回传
    
    return len;
} 