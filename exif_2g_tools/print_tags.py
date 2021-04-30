import EXIFTagsPython as et

if __name__=="__main__":
    import sys

    if (len(sys.argv) != 2):
        print ("USAGE: print_tags.py <filename>")
        sys.exit()
    
    filename = sys.argv[1]

    tags = et.Tags()
    error_message = ""
    if not tags.load_header(filename, error_message):
        print (error_message)
        sys.exit()
    
    for prop in dir(tags):
        if not ( prop[0] == '_' or prop[0] == 'P' ):
            print (f"{prop}: {getattr(tags, prop)}")