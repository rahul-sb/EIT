function closeMcu(serial_port, mcu)

fclose(serial_port);

% Remove any excess data
flushinput(serial_port);
flushoutput(serial_port);

delete(instrfind('Port',mcu.init.com_port));

end