from optparse import OptionParser
import os
import sys 

if __name__ == "__main__":
    user = os.getenv("USER")

    parser = OptionParser()

    parser.add_option('--geometry', help='geometry', default=None)
    parser.add_option('--anti_fiducial', help='anti_fiducial', default=False, action="store_true")

    (args, dummy) = parser.parse_args()

    if args.anti_fiducial:
        print("anti_fiducial_"+args.geometry)

