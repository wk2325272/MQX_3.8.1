%-
%- Implementation of RTOS ADAPTOR for MQX
%-
%- (C) 2009 Freescale, all rights reserved
%- Author: Jaroslav Cernoch
%-
%------------------------------------------------------------------------------
%- Including common RTOS adapter library.
%------------------------------------------------------------------------------
%- It is included indirectly through symbol as a workaround of limitation
%- of the tool used for creating installation.
%-
%define RTOSAdap_priv_CommonAdapterInterface Sw\RTOSAdaptor\Common_RTOSAdaptor.prg
%include %'RTOSAdap_priv_CommonAdapterInterface'
%undef RTOSAdap_priv_CommonAdapterInterface
%-
%-
%------------------------------------------------------------------------------
%- list RTOSAdap_priv_[simple]componentTypeAttribs
%------------------------------------------------------------------------------
%- Contains for each enumerated component type string of format:
%- "|componentType=<componentType>|installFunctionName=<installFunctionName>|"
%- Indices in this list must match the indices within list RTOSAdap_enum_componentTypes.
%-
%define_list RTOSAdap_priv_componentTypeAttribs {
|componentType=HAL_UART_Polling|installFunctionName=_io_serial_polled_install|
|componentType=HAL_UART_Int|installFunctionName=_io_serial_int_install|
|componentType=HAL_I2C_Polling|installFunctionName=_io_i2c_polled_install|
|componentType=HAL_I2C_Int|installFunctionName=_io_i2c_int_install|
|componentType=HAL_GPIO|installFunctionName=_io_gpio_install|
|componentType=HAL_ADC|installFunctionName=_io_adc_install|
%define_list}
%-
%define_list RTOSAdap_priv_simpleComponentTypeAttribs {
|simpleComponentType=HAL_UART|mqxIOType=IO_TYPE_SERIAL|
|simpleComponentType=HAL_I2C|mqxIOType=IO_TYPE_I2C|
|simpleComponentType=HAL_GPIO|mqxIOType=IO_TYPE_GPIO|
|simpleComponentType=HAL_ADC|mqxIOType=IO_TYPE_ADC|
|simpleComponentType=HAL_RTC|mqxIOType=|
|simpleComponentType=HAL_Ethernet|mqxIOType=|
%define_list}
%-
%- UART Polling and interrupt-driven
%define_list RTOSAdap_priv_API_HAL_UART_Polling {
Init
Deinit
RecvChar
SendChar
Ioctl
%define_list}
%-
%define_list RTOSAdap_priv_API_HAL_UART_Int {
Init
Deinit
RecvChar
SendChar
Ioctl
%define_list}
%-
%- I2C Polling and Interrupt-driven
%define_list RTOSAdap_priv_API_HAL_I2C_Polling {
Init
Deinit
RecvBlock
SendBlock
Ioctl
%define_list}
%-
%define_list RTOSAdap_priv_API_HAL_I2C_Int {
Init
Deinit
RecvBlock
SendBlock
Ioctl
%define_list}
%-
%- GPIO
%define_list RTOSAdap_priv_API_HAL_GPIO {
Init
Deinit
Ioctl
%define_list}
%-
%- HAL_ADC has no installation function
%define_list RTOSAdap_priv_API_HAL_ADC {
Init
Deinit
GetMeasuredValues
Ioctl
%define_list}
%-
%- RTC has no installation function
%define_list RTOSAdap_priv_API_HAL_RTC {
%define_list}
%-
%- Ethernet has no installation function
%define_list RTOSAdap_priv_API_HAL_Ethernet {
%define_list}
%-
%- CAN has no installation function
%define_list RTOSAdap_priv_API_HAL_CAN {
%define_list}
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genRTOSTypeDefinitions
%------------------------------------------------------------------------------
%--  %- PUBLIC TYPES
%--%{ {%'OperatingSystemId' RTOS Adapter} Pointer to the device data structure used and managed by RTOS %}
%--typedef void *LDD_RTOS_TDeviceDataPtr;
%--%{ {%'OperatingSystemId' RTOS Adapter} RTOS specific definition of type of Ioctl() command constants %}
%--typedef unsigned long LDD_RTOS_TIoctlCommand;
%--
  %- PRIVATE TYPES
%{ {%'OperatingSystemId' RTOS Adapter} Type of the parameter passed into ISR from RTOS interrupt dispatcher %}
typedef void *LDD_RTOS_TISRParameter;

%{ {%'OperatingSystemId' RTOS Adapter} Structure for saving/restoring interrupt vector %}
typedef struct {
  void (*isrFunction)(LDD_RTOS_TISRParameter);                   %>>%{ ISR function handler %}
  LDD_RTOS_TISRParameter isrData;                                %>>%{ ISR parameter %}
} LDD_RTOS_TISRVectorSettings;
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_priv_getInstallationFunctionForDriverType(arg_componentType,out_installFunctionName)
%------------------------------------------------------------------------------
%- Private method. Returns the name of RTOS installation function for the specified
%- component type.
%-
%- Input parameters:
%-      arg_componentType       - Type of the component. One from enumeration list 'RTOSAdap_enum_componentTypes'.
%-
%- Outputs:
%-      out_installFunctionName - Name of RTOS function installing the driver of component
%-                                of specified type.
%-
  %- Check if arg_componentType is a valid enumerated value
  %:loc_compTypeIndex = %get_index1(%'arg_componentType',RTOSAdap_enum_componentTypes)
  %if loc_compTypeIndex = '-1'
    %error! Invalid HAL component type "%'arg_componentType'"
  %endif
  %-
  %- In RTOSAdap_priv_componentTypeAttribs there is item of format "<componentType>|<installFunctionName>"
  %- for each component type on the same index as within the list RTOSAdap_enum_componentTypes.
  %define loc_compTypeAttribs %[%'loc_compTypeIndex',RTOSAdap_priv_componentTypeAttribs]
  %inclSUB RTOSAdap_lib_getStructMember(%'loc_compTypeAttribs',componentType,loc_checkCompType)
  %inclSUB RTOSAdap_lib_getStructMember(%'loc_compTypeAttribs',installFunctionName,%'out_installFunctionName')
  %if loc_checkCompType != arg_componentType
    %error! %'OperatingSystemId' RTOS Adapter internal error, lists RTOSAdap_priv_componentTypeAttribs and RTOSAdap_componentTypes are not in index-consistency (at index %'loc_compTypeIndex' there is "%'loc_checkCompType'" instead of "%'arg_componentType'")
  %endif
  %-
  %undef loc_compTypeIndex
  %undef loc_compTypeAttribs
  %undef loc_checkCompType
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genDriverInstallationsFunctionDefinition
%------------------------------------------------------------------------------
%- MQX implementation: For each instance of component calls RTOS installation function
%- (specific for each type of component). The name of the component instance is passed as first parameter.
%- Then pointers to all registered method implementations are passed as next parameters
%- (in the same order they have been registered).
%-
%define! Description_PE_Install_Hardware_Drivers The function responsible for installing drivers for all HAL components within the project.
%include Common\GeneralInternalGlobal.Inc (PE_Install_Hardware_Drivers)
%{ {%'OperatingSystemId' RTOS Adapter} Definition of main function for installation of drivers for all components %}
void PE_Install_Hardware_Drivers (void)
{
  %- List PE_G_RTOSAdap_installFuns contains all names of lists of lines of installation function calls
  %for loc_instanceName from PE_G_RTOSAdap_regCompInstanceNames
    %- Obtain component type
    %define! loc_componentType %[%'for_index_1',PE_G_RTOSAdap_regCompInstanceTypes]
    %ifdef RTOSAdap_priv_API_%'loc_componentType'
      %if %list_size(RTOSAdap_priv_API_%'loc_componentType') > '0'
        %- If list is empty, no installation function will be called
        %-
        %- Get installation function for arg_componentType
        %inclSUB RTOSAdap_priv_getInstallationFunctionForDriverType(%'loc_componentType',loc_installFunctionName)
        %-
  %{ {%'OperatingSystemId' RTOS Adapter} Call to the driver installation function for component "%'loc_instanceName'" %}
  (void)%'loc_installFunctionName'(
        %- First argument: driver name = component instance name + ':'
    "%'loc_instanceName':", %{ Driver name equals to the name of the component instance %}
        %-
        %- Next arguments are pointers to methods
        %for loc_methodName from RTOSAdap_priv_API_%'loc_componentType'
          %ifdef PE_G_RTOSAdap_regCompInstance_%'loc_instanceName'_methodPtr_%'loc_methodName'
            %if %for_last
    %{ %'loc_methodName' %} %"PE_G_RTOSAdap_regCompInstance_%'loc_instanceName'_methodPtr_%'loc_methodName'"
            %else
    %{ %'loc_methodName' %} %"PE_G_RTOSAdap_regCompInstance_%'loc_instanceName'_methodPtr_%'loc_methodName'",
            %endif
          %else
            %error! Method "%'loc_methodName'" is not registered for HAL component "%'loc_instanceName'" of type "%'loc_componentType'" (check call to RTOSAdap_registerComponentMethodImplementation() from HAL component DRV file)
          %endif
        %endfor  %- loc_methodName
        %- Add closing ")" to the installation function call
  );
      %endif %- list_size(RTOSAdap_priv_API_%'loc_componentType')>'0'
    %else
      %error! API for HAL component of type "%'loc_componentType'" is not defined. Check definition of list RTOSAdap_priv_API_%'loc_componentType' within %'OperatingSystemId' RTOS Adapter.
    %endif
  %endfor %- loc_instanceName
  %-
  %undef! loc_componentType
  %undef! loc_installFunctionName
}
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genDriverInstallationsFunctionDeclaration
%------------------------------------------------------------------------------
%include Common\GeneralInternalGlobal.Inc (PE_Install_Hardware_Drivers)
%{ {%'OperatingSystemId' RTOS Adapter} Prototype of main function for installation of drivers of all components %}
void PE_Install_Hardware_Drivers (void);
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genDriverMemoryAlloc(arg_destPtrBuffer,arg_objType,opt_arg_errCode,arg_globDefsThread)
%------------------------------------------------------------------------------
  %- Check if RTOSAdap_driverMemoryAlloc() was called only once for this object
  %if defined(RTOSAdap_alloc_object_%'arg_destPtrBuffer')
    %error! There was called allocation for object "%'arg_destPtrBuffer'" for object type "%'arg_objType'" twice (which does not work for bareboard RTOS what only simulates the dynamic allocation)
  %else
    %- o.k., it is first call, mark that such object was allocated
    %define RTOSAdap_alloc_object_%'arg_destPtrBuffer'
  %endif
  %-
  %- Dynamic allocation is supported (generate it)
  %inclSUB RTOSAdap_getRTOSFunction(Malloc,loc_MallocFunction)
%{ {%'OperatingSystemId' RTOS Adapter} Driver memory allocation: RTOS function call is defined by %'OperatingSystemId' RTOS Adapter property %}
%'arg_destPtrBuffer' = (%'arg_objType' *)%'loc_MallocFunction'((_mem_size)sizeof(%'arg_objType'));
  %if opt_arg_errCode<>''
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
  if (%'arg_destPtrBuffer' == NULL) {
    %'opt_arg_errCode'
  }
#endif
  %endif
  %-
  %- Set memory block type (not supported now)
%-_mem_set_type(%'arg_destPtrBuffer', MEM_TYPE_...);
  %-
  %undef loc_MallocFunction
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genDriverMemoryDealloc(arg_ptrBuffer,arg_objType)
%------------------------------------------------------------------------------
  %- Dynamic allocation is supported, generate it
  %inclSUB RTOSAdap_getRTOSFunction(Dealloc,loc_DeallocFunction)
%{ {%'OperatingSystemId' RTOS Adapter} Driver memory deallocation: RTOS function call is defined by %'OperatingSystemId' RTOS Adapter property %}
%'loc_DeallocFunction'(%'arg_ptrBuffer');
  %-
  %undef loc_DeallocFunction
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_getInterruptVectorSymbol(arg_vectorName,out_vectorIndexConstantName)
%------------------------------------------------------------------------------
  %define! %'out_vectorIndexConstantName' LDD_ivIndex_%'arg_vectorName'
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genSetInterruptVector(arg_intVectorProperty,arg_isrFunctionName,arg_isrParameterType,arg_isrParameterValue,opt_arg_oldISRSettings,arg_globDefsThread)
%------------------------------------------------------------------------------
%- MQX 'arg_isrParameterType' limitation: Parameter type should be convertible to 'void *' type.
%-
  %- Check if RTOSAdap_genSetInterruptVector() was called only once for this interrupt vector
  %if defined(RTOSAdap_alloc_interrupt_%"%'arg_intVectorProperty'_Name")
    %error! There was called allocation for interrupt vector %"%'arg_intVectorProperty'_Name" twice (which does not work for bareboard RTOS what only simulates the run-time vector allocation)
  %else
    %- o.k., it is first call, mark that interrupt vector was allocated
    %define RTOSAdap_alloc_interrupt_%"%'arg_intVectorProperty'_Name"
  %endif
  %-
  %- MQX has support of run-time installable ISR vectors
  %-
  %define! loc_vectorIndexConstantName
  %inclSUB RTOSAdap_getInterruptVectorSymbol(%"%'arg_intVectorProperty'_Name",loc_vectorIndexConstantName)
  %-
  %if opt_arg_oldISRSettings != ''
%{ {%'OperatingSystemId' RTOS Adapter} Save old and set new interrupt vector (function handler and ISR parameter) %}
%{ Note: Exception handler for interrupt is not saved, because it is not modified %}
%opt_arg_oldISRSettings.isrData = _int_get_isr_data(%loc_vectorIndexConstantName);
%opt_arg_oldISRSettings.isrFunction = _int_install_isr(%loc_vectorIndexConstantName, %arg_isrFunctionName, %arg_isrParameterValue);
  %else
%{ {%'OperatingSystemId' RTOS Adapter} Set new interrupt vector (function handler and ISR parameter) %}
(void)_int_install_isr(%loc_vectorIndexConstantName, %arg_isrFunctionName, %arg_isrParameterValue);
  %endif
  %-
  %undef loc_vectorIndexConstantName
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genRestoreInterruptVector(arg_intVectorProperty,arg_oldISRSettings)
%------------------------------------------------------------------------------
  %inclSUB RTOSAdap_getInterruptVectorSymbol(%"%'arg_intVectorProperty'_Name",loc_vectorIndexConstantName)
  %-
%{ {%'OperatingSystemId' RTOS Adapter} Restore interrupt vector (function handler and ISR parameter) %}
%{ Note: Exception handler for interrupt is not restored, because it was not modified %}
(void)_int_install_isr(%loc_vectorIndexConstantName, %arg_oldISRSettings.isrFunction, %arg_oldISRSettings.isrData);
  %-
  %undef loc_vectorIndexConstantName
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genISRSettingsVarDeclaration(arg_varName,opt_arg_comment)
%------------------------------------------------------------------------------
  %if opt_arg_comment != ''
LDD_RTOS_TISRVectorSettings %'arg_varName';                      %>>%{ {%'OperatingSystemId' RTOS Adapter} %'opt_arg_comment' %}
  %else
LDD_RTOS_TISRVectorSettings %'arg_varName';                      %>>%{ {%'OperatingSystemId' RTOS Adapter} Buffer where old interrupt settings are saved %}
  %endif
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genISRFunctionDefinitionOpen(arg_intVectorProperty,arg_isrFunctionName,arg_isrParameterType,arg_isrParameterName)
%------------------------------------------------------------------------------
%- MQX 'arg_isrParameterType' limitation: Parameter type should be convertible to 'void *' type.
%-
void %'arg_isrFunctionName'(LDD_RTOS_TISRParameter _isrParameter)
{
  %{ {%'OperatingSystemId' RTOS Adapter} ISR parameter is passed as parameter from RTOS interrupt dispatcher %}
  %'arg_isrParameterType' %'arg_isrParameterName' = (%'arg_isrParameterType')_isrParameter;
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genISRFunctionDefinitionClose(arg_intVectorProperty,arg_isrFunctionName,arg_isrParameterType,arg_isrParameterName)
%------------------------------------------------------------------------------
}
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genISRFunctionDeclaration(arg_intVectorProperty,arg_isrFunctionName,arg_isrParameterType,arg_isrParameterName)
%------------------------------------------------------------------------------
%- MQX 'arg_isrParameterType' limitation: Parameter type should be convertible to 'void *' type.
%-
%{ {%'OperatingSystemId' RTOS Adapter} ISR function prototype %}
void %'arg_isrFunctionName'(LDD_RTOS_TISRParameter _isrParameter);
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genReentrantMethodBegin(opt_arg_genReentrantMethods)
%------------------------------------------------------------------------------
  %- Handle optional parameter
  %inclSUB RTOSAdap_priv_getOptionalParameterEffectiveValue(genReentrantMethods,%'opt_arg_genReentrantMethods',arg_genReentrantMethods)
  %-
  %if arg_genReentrantMethods == 'yes'
    %- Reentrant methods are required, generate enter critical section
    %inclSUB RTOSAdap_getRTOSFunction(EnterCritical,loc_enterCriticalFunctionName)
%{ {%'OperatingSystemId' RTOS Adapter} Method is required to be reentrant, critical section begins (RTOS function call is defined by %'OperatingSystemId' RTOS Adapter property) %}
%'loc_enterCriticalFunctionName'();
    %undef loc_enterCriticalFunctionName
  %elif arg_genReentrantMethods == 'no'
    %- Reentrant methods are not required, no code is generated
  %else
    %error! Invalid value of parameter 'opt_arg_genReentrantMethods' ("%'arg_genReentrantMethods'")
  %endif
  %-
  %undef arg_genReentrantMethods
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genReentrantMethodEnd(opt_arg_genReentrantMethods)
%------------------------------------------------------------------------------
  %- Handle optional parameter
  %inclSUB RTOSAdap_priv_getOptionalParameterEffectiveValue(genReentrantMethods,%'opt_arg_genReentrantMethods',arg_genReentrantMethods)
  %-
  %if arg_genReentrantMethods == 'yes'
    %- Reentrant methods are required, generate exit critical section
    %inclSUB RTOSAdap_getRTOSFunction(ExitCritical,loc_exitCriticalFunctionName)
%{ {%'OperatingSystemId' RTOS Adapter} Critical section end (RTOS function call is defined by %'OperatingSystemId' RTOS Adapter property) %}
%'loc_exitCriticalFunctionName'();
    %undef loc_exitCriticalFunctionName
  %elif arg_genReentrantMethods == 'no'
    %- Reentrant methods are not required, no code is generated
  %else
    %error! Invalid value of parameter 'opt_arg_genReentrantMethods' ("%'arg_genReentrantMethods'")
  %endif
  %-
  %undef arg_genReentrantMethods
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genCriticalSectionBegin(opt_arg_genReentrantMethods)
%------------------------------------------------------------------------------
  %- Handle optional parameter
  %inclSUB RTOSAdap_priv_getOptionalParameterEffectiveValue(genReentrantMethods,%'opt_arg_genReentrantMethods',arg_genReentrantMethods)
  %-
  %if arg_genReentrantMethods == 'yes'
    %- The method itself is generated as reentrant, additional critical sections are not needed
  %elif arg_genReentrantMethods == 'no'
    %- Reentrant methods are not required, but critical sections must remain
    %inclSUB RTOSAdap_getRTOSFunction(EnterCritical,loc_enterCriticalFunctionName)
%{ {%'OperatingSystemId' RTOS Adapter} Critical section begin (RTOS function call is defined by %'OperatingSystemId' RTOS Adapter property) %}
%'loc_enterCriticalFunctionName'();
    %undef loc_enterCriticalFunctionName
  %else
    %error! Invalid value of parameter 'opt_arg_genReentrantMethods' ("%'arg_genReentrantMethods'")
  %endif
  %-
  %undef arg_genReentrantMethods
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genCriticalSectionEnd(opt_arg_genReentrantMethods)
%------------------------------------------------------------------------------
  %- Handle optional parameter
  %inclSUB RTOSAdap_priv_getOptionalParameterEffectiveValue(genReentrantMethods,%'opt_arg_genReentrantMethods',arg_genReentrantMethods)
  %-
  %if arg_genReentrantMethods == 'yes'
    %- The method itself is generated as reentrant, additional critical sections are not needed
  %elif arg_genReentrantMethods == 'no'
    %- Reentrant methods are not required, but critical sections must remain
    %inclSUB RTOSAdap_getRTOSFunction(ExitCritical,loc_exitCriticalFunctionName)
%{ {%'OperatingSystemId' RTOS Adapter} Critical section ends (RTOS function call is defined by %'OperatingSystemId' RTOS Adapter property) %}
%'loc_exitCriticalFunctionName'();
    %undef loc_exitCriticalFunctionName
  %else
    %error! Invalid value of parameter 'opt_arg_genReentrantMethods' ("%'arg_genReentrantMethods'")
  %endif
  %-
  %undef arg_genReentrantMethods
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genIoctlCommandConstantDefinition(arg_commandName,arg_commandLogicalIndex,opt_arg_simpleComponentType)
%------------------------------------------------------------------------------
  %- Get constant name
  %inclSUB RTOSAdap_getIoctlCommandConstantName(%'arg_commandName',%'opt_arg_simpleComponentType',loc_constantName)
  %- Handle optional parameters
  %inclSUB RTOSAdap_priv_getOptionalParameterEffectiveValue(simpleComponentType,%'opt_arg_simpleComponentType',arg_simpleComponentType)
  %- Get MQX IO type for specified simple component type
  %:loc_simpleCompTypeIndex = %get_index1(%'arg_simpleComponentType',RTOSAdap_enum_simpleComponentTypes)
  %if loc_simpleCompTypeIndex = '-1'
    %error! Invalid HAL simple component type "%'arg_simpleComponentType'"
  %endif
  %define loc_simpleCompTypeAttribs %[%'loc_simpleCompTypeIndex',RTOSAdap_priv_simpleComponentTypeAttribs]
  %inclSUB RTOSAdap_lib_getStructMember(%'loc_simpleCompTypeAttribs',simpleComponentType,loc_checkSimpleCompType)
  %inclSUB RTOSAdap_lib_getStructMember(%'loc_simpleCompTypeAttribs',mqxIOType,loc_mqxIOType)
  %if loc_checkSimpleCompType != arg_simpleComponentType
    %error! %'OperatingSystemId' RTOS Adapter internal error, lists RTOSAdap_priv_simpleComponentTypeAttribs and RTOSAdap_simpleComponentTypes are not in index-consistency (at index %'loc_simpleCompTypeIndex' there is "%'loc_checkSimpleCompType'" instead of "%'arg_simpleComponentType'")
  %endif
  %if loc_mqxIOType == ''
    %error! %'OperatingSystemId' RTOS Adapter does not define MQX IO Type for component type "%'arg_simpleComponentType'"
  %endif
  %-
#define %'loc_constantName'                                      %>57_IO(%'loc_mqxIOType',%'arg_commandLogicalIndex')
  %-
  %undef arg_simpleComponentType
  %undef loc_constantName
  %undef loc_simpleCompTypeIndex
  %undef loc_simpleCompTypeAttribs
  %undef loc_checkSimpleCompType
  %undef loc_mqxIOType
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_getIoctlCommandConstantName(arg_commandName,opt_arg_simpleComponentType,out_constantName)
%------------------------------------------------------------------------------
  %- Handle optional parameters
  %inclSUB RTOSAdap_priv_getOptionalParameterEffectiveValue(simpleComponentType,%'opt_arg_simpleComponentType',arg_simpleComponentType)
  %-
  %- Check if arg_componentType is a valid enumerated value
  %:loc_simpleCompTypeIndex = %get_index1(%'arg_simpleComponentType',RTOSAdap_enum_simpleComponentTypes)
  %if loc_simpleCompTypeIndex = '-1'
    %error! Invalid HAL simple component type "%'arg_simpleComponentType'"
  %endif
  %-
  %define! %'out_constantName' %'arg_simpleComponentType'_IOCTL_%'arg_commandName'
  %-
  %undef arg_simpleComponentType
  %undef loc_simpleCompTypeIndex
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genRTOSIncludes()
%------------------------------------------------------------------------------
#include "mqx.h"
#include "ioctl.h"
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genRTOSDriverIncludes()
%------------------------------------------------------------------------------
%ifdef PE_G_RTOSAdap_regCompInstanceTypes
  %if ('HAL_UART_Polling' in PE_G_RTOSAdap_regCompInstanceTypes) || ('HAL_UART_Int' in PE_G_RTOSAdap_regCompInstanceTypes)
#include "serial.h"
  %endif
  %if ('HAL_GPIO' in PE_G_RTOSAdap_regCompInstanceTypes)
#include "io_gpio.h"
  %endif
  %if ('HAL_ADC' in PE_G_RTOSAdap_regCompInstanceTypes)
#include "adc.h"
  %endif
  %if ('HAL_I2C_Polling' in PE_G_RTOSAdap_regCompInstanceTypes) || ('HAL_I2C_Int' in PE_G_RTOSAdap_regCompInstanceTypes)
#include "i2c.h"
  %endif
%endif
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genPublishedRTOSSettings
%------------------------------------------------------------------------------
  %-
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genMemoryBarrierBegin(opt_arg_genReentrantMethods)
%------------------------------------------------------------------------------
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genMemoryBarrierEnd(opt_arg_genReentrantMethods)
%------------------------------------------------------------------------------
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genReentrantMethodDeclare(opt_arg_genReentrantMethods, spinlock_name)
%------------------------------------------------------------------------------
 %- Do nothing for bareboard 
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genRTOSReturnIRQ_Attended()
%------------------------------------------------------------------------------
return;
%SUBROUTINE_END
%-
%-
%------------------------------------------------------------------------------
%SUBROUTINE RTOSAdap_genRTOSReturnIRQ_NOTAttended()
%------------------------------------------------------------------------------
return;
%SUBROUTINE_END
%-
%-