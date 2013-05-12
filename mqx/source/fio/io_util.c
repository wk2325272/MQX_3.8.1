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
* $FileName: io_util.c$
* $Version : 3.8.11.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Contains misc io functions.
*
*END************************************************************************/

#include "mqx_inc.h"
#include "fio.h"
#include <ctype.h>
#include <string.h>


static char_ptr _parse_next_filename(char_ptr src, int_32_ptr len, int_32 max_len); 

static FS_TABLE_ENTRY _opened_fs_table[MAX_FS_INSTANCES]; 
boolean _io_strtolower( char_ptr arg)
{
   int_32 i=0;
   
   if (arg == NULL) return FALSE;
   while (arg[i])  {
      // Allows constant strings - as long as they are lowercase already
      if (isupper(arg[i])) {
        arg[i] = (char) tolower(arg[i]);
      }
      i++;
   }
   return FALSE;
}

_mqx_int _io_strcasecmp(const char *s1, const char *s2)
{
   while (tolower((int)*s1) == tolower((int)*s2))
   {
      if (*s1++ == '\0')
         return (0);
      s2++;
   }
   return (tolower((int)*s1) - tolower((int)*s2));
}

_mqx_int _io_strncasecmp(const char *s1, const char *s2, uint_32 n)
{
   if (n != 0)
   {
      do 
      {
         if (tolower((int)*s1) != tolower((int)*s2))
            return (tolower((int)*s1) - tolower((int)*s2));
         if (*s1++ == '\0')
            break;
         s2++;
      } while (--n != 0);
   }
   return (0);
}

char_ptr _io_find_filename(char_ptr arg)  
{
   uint_32 i=0;

   if (arg==NULL) return NULL;
   
   while (isgraph(arg[i]) && (arg[i] != ':')) i++;
   if (arg[i] == ':')  {
      return &arg[i+1];
   } else  if (arg[i] == '\0')  {
      return arg;
   } else  {
      return NULL;
   }
}


boolean _io_validate_device(char_ptr arg)  
{
   uint_32 i=0;

   while (isgraph(arg[i]) && (arg[i] != ':')) i++;
   if (i && (arg[i]==':') && (arg[i+1]=='\0')) return TRUE;
   return FALSE;
}


void _io_create_prefixed_filename( char_ptr new_ptr, char_ptr in_ptr,  char_ptr dev_ptr)
{
   char_ptr file_ptr;
   
   new_ptr[0] = '\0';
   file_ptr = _io_find_filename(in_ptr);
   if (file_ptr == in_ptr)  {
      // No device name supplied, use current
      strcpy(new_ptr, dev_ptr);
   }  
   strcat(new_ptr, in_ptr);
}
/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _io_get_fs_by_name
* Returned Value   : valid FS_TABLE_ENTRY or NULL
* Comments         :
*    returns pointer on the row where fs_name is stored
*
*END*-----------------------------------------------------------------*/
MQX_FILE_PTR _io_get_fs_by_name(char_ptr fs_name)
{
   int i;
   
   if (*fs_name == '\0')
     return NULL;
   
   for(i=0;i<MAX_FS_INSTANCES;i++)
   {
      if (strcmp(_opened_fs_table[i].FS_NAME, fs_name) == 0)
      {
         return _opened_fs_table[i].FS_PTR;      
      }
   }
   // filesystem not found in table
   return NULL;
}
/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _io_get_first_valid_fs
* Returned Value   : valid FS_TABLE_ENTRY or NULL
* Comments         :
*    Returns pointer on the row where first valid filesystem is stored 
*
*END*-----------------------------------------------------------------*/
MQX_FILE_PTR _io_get_first_valid_fs()
{
   int i;
   // find first used field in table
   for(i=0;i<MAX_FS_INSTANCES;i++)
   {
      if (_opened_fs_table[i].FS_PTR != NULL)
      {
         return _opened_fs_table[i].FS_PTR; 
      }
   }
   return NULL;
}

/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _io_is_fs_valid
* Returned Value   : valid TRUE or FALSE
* Comments         :
*    Return TRUE or FALSE if filesystem is in list or not
*
*END*-----------------------------------------------------------------*/
boolean _io_is_fs_valid(MQX_FILE_PTR fs_ptr)
{
   int i;
   
   /* First check that file pointer is not null */
   if (fs_ptr == NULL)
   {
      return FALSE;    
   }
      
   /* find first used field in table */
   for(i=0;i<MAX_FS_INSTANCES;i++)
   {
      if (_opened_fs_table[i].FS_PTR == fs_ptr)
      {
         return TRUE;
      }
   }
   return FALSE;
}
/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _io_get_fs_name
* Returned Value   : MQX_OK or ERROR CODE 
* Comments         :
*    Return MQX_OK or ERROR CODE if filesystem is in list or not
*
*END*-----------------------------------------------------------------*/
int_32 _io_get_fs_name(MQX_FILE_PTR fs_ptr, char_ptr fs_name, int_32 fs_name_len)
{
   int i;
 
    /* First check that input parameters are not not null */
   if (fs_ptr == NULL || fs_name == NULL)
   {
      return MQX_INVALID_POINTER;    
   }
   
   /* find first used field in table */
   for(i=0;i<MAX_FS_INSTANCES;i++)
   {
      if (_opened_fs_table[i].FS_PTR == fs_ptr)
      {
         if(fs_name_len > strlen(_opened_fs_table[i].FS_NAME))
         {
           strcpy(fs_name,_opened_fs_table[i].FS_NAME);
           return MQX_OK;
         }
         else
         {
           return MQX_INVALID_SIZE;
         }
      }
   }
   return MQX_INVALID_POINTER;
}
/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _io_register_file_system
* Returned Value   : MQX_OK or an error code
* Comments         :
*    Store filesystem name and handle into _opened_fs_table 
*
*END*-----------------------------------------------------------------*/

uint_32 _io_register_file_system(MQX_FILE_PTR fd_ptr,char_ptr name_ptr)
{
   int i;

   /* First check that file pointer is not null */
   if (fd_ptr == NULL || name_ptr == NULL)
   {
      return MQX_INVALID_POINTER;    
   }       
   
   
   /* find first empty field in table */
   for(i=0;i<MAX_FS_INSTANCES;i++)
   {
      if (_opened_fs_table[i].FS_PTR == NULL)
      {
        /* store filesystem pointer and name into the table */
        _opened_fs_table[i].FS_PTR = fd_ptr;
         strncpy(_opened_fs_table[i].FS_NAME, name_ptr, FS_MAX_DEVLEN);
         return MQX_OK;      
      }
   }
   /* there is no empty slot, report error */
   return MQX_OUT_OF_MEMORY;
}

/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _io_unregister_file_system
* Returned Value   : MQX_OK or an error code
* Comments         :
*    Remove filesystem name and handle from _opened_fs_table 
*
*END*-----------------------------------------------------------------*/
uint_32 _io_unregister_file_system(MQX_FILE_PTR fd_ptr)
{
   int i;

   /* First check that file pointer is not null */
   if (fd_ptr == NULL)
   {
      return MQX_INVALID_POINTER;    
   }    
   
   // find first empty field in table
   for(i=0;i<MAX_FS_INSTANCES;i++)
   {
      if (_opened_fs_table[i].FS_PTR == fd_ptr)
      {
         // store filesystem pointer and name into the table
         _opened_fs_table[i].FS_PTR = NULL;
         _opened_fs_table[i].FS_NAME[0] ='\0' ;
         // return MQX_OK
         return MQX_OK;      
      }
   }
   // filesystem not found report error
   return MQX_INVALID_HANDLE;
}

/*FUNCTION*-------------------------------------------------------------------
 * _io_get_fs_for_path
 * 
 *   functions parse given path and returns device name eather from input path
 *   or curent device name(if there was no dev name in input path) 
 *   
 * params: input_path      - pointer to parsed path
 *         out_dev         - [out]pointer to place where device: will be stored
 *         dev_len             - size of outputpath
 *         cur_dev         - current device name
 *         is_dev_in_path  - [out] True/False - device name found in path
 *         return value    - [out]length of device name       
*END*---------------------------------------------------------------------*/
int_32 _io_get_dev_for_path(char_ptr out_dev, boolean * is_dev_in_path, int_32 dev_len, char_ptr input_path, char_ptr cur_dev)
{
   int i = 0;
   if (input_path==NULL) return MQX_INVALID_POINTER;
   
   while (i < (dev_len-1) && isgraph(input_path[i]) && (input_path[i] != ':')) i++;
   i++;
   if (input_path[i-1] == ':' && (input_path[i] == '\\' || input_path[i] == '/'))  
   {
      // there is device name in input path 
      if (is_dev_in_path != NULL)
      {
        *is_dev_in_path = TRUE;
      }
      strncpy(out_dev,input_path,i);
      out_dev[i] = '\0';
      return i;
   } 
   else 
   {
      // device name is NOT in input path
      if (is_dev_in_path != NULL)
      {
        *is_dev_in_path = FALSE;
      }
      i = strlen(cur_dev);
      if (i < (dev_len - 1))
      {
         strcpy(out_dev,cur_dev);
         return i;
      }
      else 
      {
         return 0;
      }
   }
}



/*FUNCTION*-------------------------------------------------------------------
 * convert relative to absolute path and extend by device name if not present
 * params: inputpath    - pointer to parsed path
 *         result       - [out]pointer to place where abs. path will be stored
 *         len          - size of result
 *         cur_dev      - current device name
 *         cirdir       - current directory name
 *         return value - [out]OK or error code   
 *END*---------------------------------------------------------------------*/
int_32 _io_rel2abs(char_ptr result, char_ptr curdir, char_ptr inputpath, int_32 len, char_ptr cur_dev)
{
  int_32 inputpathlen = strlen(inputpath);
  char_ptr path;
  char_ptr dest;
  char_ptr dirstart;  
  int_32 length = 0, devlen = 0,dirlen = 0,error_code = 0;
  boolean  relpathflag = FALSE, is_dev_in_path =  FALSE;

  devlen =  _io_get_dev_for_path(result,&is_dev_in_path,len,inputpath,cur_dev);

  // device name during path parsing will be skiped 
  dest = result = result + devlen;
  if(is_dev_in_path == TRUE)
  {
    // there was device name in input path -> skip it in parsing
    inputpath = inputpath + devlen;
  }
  dest[0] = '\\';
  dest[1] = '\0';
  length = 1;

 
  // selected path begins with / or \, that means absolute path
  if(*inputpath == '/' || *inputpath == '\\')
  {
    path = inputpath;
    relpathflag = 0;
  }
  else
  {
    path = curdir+1;
    relpathflag = 1;
  }

  // browse path, convert to upper case and eliminate . and ..
  while ( path )
  {
    if ( *path )
    {
      dirstart = path;
      path = _parse_next_filename(path, &dirlen, len);
      if (path != NULL && dirstart[0] == '.' && dirstart[1] == '.' )  // ".." 
      {
        if ( dest != result )
        {
          while ( dest != result && *dest != '\\' )
          {
            dest--;
            length--;
          }
          /* Erase the previous backslash, unless it is the
          ** first (root directory).
          */
          if ( dest != result )
          {
            *dest-- = '\0';
            length--;
          }
          else                                        
          {                                           
             *(dest+1) = '\0';                        
          }                                           
        }                                             
        else                                          
        {                                             
          /*                                          
          ** The check for underflow is redundant, because the path
          ** has been found already.
          */
          error_code = MQX_INVALID_PARAMETER;
          break;
        }
      }
      else if ( dirstart[0] && dirstart[0] != '.' ) // not dotted path
      {
        if ( *dest != '\\' )
        {
          *(++dest) = '\\';
          length++;
        }
        length += dirlen;
        if ( length < len )
        {
          strncpy (++dest, dirstart, dirlen);
          dest[dirlen] = '\0';
          dest = result + length - 1;
        }
        else
        {
          error_code = MQX_INVALID_PARAMETER;
          break;
        }
      }
    }
    // the end of parsed string check relative/absolute path
    else
    {
      
      if(relpathflag == 1)
      {
        // new path was relative(did not started with / or \), continue in process
        path = inputpath;
        relpathflag = 0;
      }
      else
      {
        // absolute path finish process 
        break;
      }
    }
  }
  return error_code;
}



/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  _parse_next_filename
* Returned Value   :  char_ptr to where it left off (non / or \ char)
* Comments  :
*        Reads the first filename in source string, up to '\' ,'/' or '.'
*        If the filename we're reading is invalid, return NULL
*END*---------------------------------------------------------------------*/

static char_ptr _parse_next_filename
    (
    char_ptr   src,     /*[IN]  the pathanme */
    int_32_ptr len,     /*[OUT] length of the filename just read in */
    int_32     max_len  /*[IN]  max allowend length for filename*/
    )
{
    *len = 0;

    while ( *src && *src != '/' && *src != '\\' )
    {
        src++;
        (*len)++;
        if ( *len > max_len )
        {
            *len = 0;
            return NULL;
        }
    }  

    if ( *src == '\\' || *src == '/' )
    {
        src++;
    }

    return(src);
}  

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  _io_atoi
* Returned Value   :  conveted string to int or 0 if error
* Comments  : converts ascii to string
*END*---------------------------------------------------------------------*/

_mqx_int _io_atoi(const char *str) {
    int value = 0, sign = 1;
    while (*str == ' ') ++str; // skip leading whitespace

   if ( *str == '-' ) {
        sign = -1;
        str++;
   }

    while ( *str) {
        if ((*str >= '0') && (*str <= '9'))
            value = 10*value + (*str - '0');
        else
            break; // non-digit
        str++;
    }
    if (value < 0) return 0; // integer overflow
    return value*sign;
}

 
 /*
static MQX_FILE_PTR _io_default_filesystem = NULL;

void _io_set_default_filesystem(MQX_FILE_PTR fp)  
{
   _io_default_filesystem = fp;
}

MQX_FILE_PTR _io_get_default_filesystem(void)  
{
   return _io_default_filesystem;
}

static char_ptr _io_default_filesystem_name = NULL;

void _io_set_default_filesystem_name(char_ptr name)  
{
   _io_default_filesystem_name = name;
}

char_ptr _io_get_default_filesystem_name(void)  
{
   return _io_default_filesystem_name;
}
*/

