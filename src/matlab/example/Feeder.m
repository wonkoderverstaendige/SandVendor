% USE:
% f = Feeder('COM7');
% f.move(5);
% f.next();
% f.close();

classdef Feeder < handle
    properties 
        serialConnection
        currentPos = 1;
    end
    methods
        function obj = Feeder(path)
            obj.serialConnection = serial(path, 'Terminator', '');
            fopen(obj.serialConnection);
        end
        
        function move(obj, newpos)
            sendval = char(newpos+48);
            obj.currentPos = newpos;
            fprintf(obj.serialConnection, sendval);
        end
        
        function close(obj)
            fclose(obj.serialConnection);
        end
        
        function next(obj)
            obj.currentPos = obj.currentPos +1;
            sendval = char(obj.currentPos+48);
            fprintf(obj.serialConnection, sendval);
        end
        
        function prev(obj)
            obj.currentPos = obj.currentPos -1;
            sendval = char(obj.currentPos+48);
            fprintf(obj.serialConnection, sendval);
        end
        
    end
end