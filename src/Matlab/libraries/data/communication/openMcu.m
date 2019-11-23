function serial_port = openMcu(mcu)

% mcu_type: 0/1 = Arduino/ARM
if mcu.mcu_type == 0
%   Delete COM port if already open.    
    delete(instrfind('Port', mcu.init.com_port)); 
    
%   Create serial port and open it.
    serial_port = serial(mcu.init.com_port,...
                         'BaudRate',mcu.init.baud_rate,...
                         'InputBufferSize',mcu.init.ip_buf_sz,...
                         'OutputBufferSize',mcu.init.op_buf_sz);
    
    fopen(serial_port);
    
%   Remove any previously present data
    flushinput(serial_port);
    flushoutput(serial_port);
else
    error("Write code for openMCU that corresponds to ARM microcontroller");
end

end