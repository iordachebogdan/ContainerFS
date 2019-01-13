import sys
import zipfile
import shutil
import tempfile
import uuid
import os


def random_filename():
    return str(uuid.uuid4())


def zipdir(path, zipf):
    curr_path = os.getcwd()
    os.chdir(path)
    for root, currdir, files in os.walk("."):
        for f in files:
            zipf.write(os.path.join(root, f))
    os.chdir(curr_path)


def gen(depth, base, num_lines, path):
    node = os.path.join(path, random_filename())
    if depth > 0:
        for _ in range(base):
            gen(depth-1, base, num_lines, node)
    else:
        # Ensure directory
        if not os.path.exists(os.path.dirname(node)):
            os.makedirs(os.path.dirname(node))

        # Write file
        with open(node, 'w+') as f:
            for i in range(num_lines):
                print(i, file=f)


def main():
    if len(sys.argv) < 4:
        print('Use python3 deep_zip.py depth base num_lines [file_name]')
        sys.exit(-1)
    depth, base, num_lines = map(int, sys.argv[1:4])
    try:
        file_name = sys.argv[4]
    except Exception:
        file_name  = 'archive.zip'

    tmp_dir = tempfile.mkdtemp()
    print("Generating in %s" % tmp_dir)
    try:
        gen(depth, base, num_lines, tmp_dir)
        zipf = zipfile.ZipFile(file_name, 'w', zipfile.ZIP_DEFLATED)
        zipdir(tmp_dir, zipf)
    finally:
        shutil.rmtree(tmp_dir)


if __name__ == '__main__':
    main()