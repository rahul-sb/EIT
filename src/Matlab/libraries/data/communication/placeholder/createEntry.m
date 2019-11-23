function placeholder = createEntry(placeholder, len, frequency)

id = datestr(now,'mmmddHHMMSSFFF'); % create a unique id

placeholder.(id).idx = 1;
placeholder.(id).len = len;
placeholder.(id).val = nan(len,frequency);

end