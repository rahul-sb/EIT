function new_data = getMcu(serial_port)

persistent prev_ascii placeholder frequency;

if isempty(frequency)
    frequency = 100; % Num of data expected in a single cycle.
    prev_ascii = 10; % Set as carriage return.
    placeholder = [];
end

% Clear the contents of the placeholder
placeholder = clearEntries(placeholder);

% Wait till data is available to collect
while ~serial_port.BytesAvailable
    pause(0.125);
end

% Read Arduino Data
ascii = fread(serial_port, serial_port.BytesAvailable);

% Account for data previously collected
ascii = [prev_ascii; ascii];

% Get the values between two carriage returns and also find the count
carriage_return = ascii == 10;
hits = find(carriage_return == 1); % gets the indices of the carriage return
num_hits = length(hits); % get the number of carriage returns

for i=1:num_hits-1
    characters = char(ascii(hits(i):hits(i+1)));    
    values = sscanf(characters,'%ld'); % Assumes only integers can be read
    len = length(values);
    
    if len>0
        placeholder_match = checkEntries(placeholder, len);        
        
        if ~any(placeholder_match)
            placeholder = createEntry(placeholder, len, frequency);
        end        
        placeholder = updateEntry(placeholder, values);
    end
end

new_data = placeholder;
prev_ascii = getRemaining(ascii, hits(end));

end



function prev_ascii = getRemaining(ascii, last_hit)
if last_hit==length(ascii)
    prev_ascii = 10; % Carriage return
else
    prev_ascii = ascii(last_hit:end);
end
end