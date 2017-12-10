#ifndef FSTREE_H_
#define FSTREE_H_

#include <time.h>
#include <vector>
#include "vmath/vmath.h"

class Dir;
class FSNode;

enum LayoutParameter {
	LP_FILE_SIZE,
	LP_FILE_SPACING,
	LP_FILE_HEIGHT,

	LP_DIR_SIZE,
	LP_DIR_SPACING,
	LP_DIR_HEIGHT,
	LP_DIR_DIST,

	NUM_LAYOUT_PARAMS
};

void set_layout_param(LayoutParameter param, float val);
float get_layout_param(LayoutParameter param);

FSNode *get_selection();

// scans the filesystem, builds the tree
bool build_tree(Dir *tree, const char *dirname);

class Link {
public:
	Dir *from, *to;
	bool selected;

	Link(Dir *from, Dir *to);

	void draw() const;
	bool intersect(const Ray &ray, float *pt) const;
};


class FSNode {
protected:
	char *name;
	size_t size;

	Vector3 vis_pos, vis_size;

	FSNode *parent;

public:
	bool selected;

	FSNode()
	{
		name = 0;
		size = 0;
		parent = 0;
		selected = false;
	}
	virtual ~FSNode()
	{
		delete[] name;
	}

	void set_name(const char *name)
	{
		delete[] this->name;
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
	}
	const char *get_name() const
	{
		return name;
	}

	void set_size(size_t sz)
	{
		size = sz;
	}
	size_t get_size() const
	{
		return size;
	}

	void set_vis_pos(const Vector3 &vpos)
	{
		vis_pos = vpos;
	}
	const Vector3 &get_vis_pos() const
	{
		return vis_pos;
	}

	void set_vis_size(const Vector3 &vsize)
	{
		vis_size = vsize;
	}
	const Vector3 &get_vis_size() const
	{
		return vis_size;
	}

	void set_parent(FSNode *p)
	{
		parent = p;
	}
	const FSNode *get_parent() const
	{
		return parent;
	}

	virtual Vector3 get_text_pos() const = 0;
	virtual float get_text_size() const = 0;

	virtual void draw() const;
	virtual bool intersect(const Ray &ray, float *pt) const;
};

enum { ATIME, MTIME, CTIME };

// --- File class ---

class File : public FSNode {
protected:
	int num_links;
	int mode, uid, gid;
	time_t time[3];

public:
	File()
	{
		num_links = 0;
		mode = 0;
		uid = gid = 0;
		time[0] = time[1] = time[2] = 0;
	}
	~File() {}

	void set_links(int nlinks)
	{
		num_links = nlinks;
	}
	int get_links() const
	{
		return num_links;
	}
	void set_mode(int mode)
	{
		this->mode = mode;
	}
	int get_mode() const
	{
		return mode;
	}
	void set_uid(int uid)
	{
		this->uid = uid;
	}
	int get_uid() const
	{
		return uid;
	}
    static const char *get_user()
    {
		return "unknown";
	}
	void set_gid(int gid)
	{
		this->gid = gid;
	}
	int get_gid() const
	{
		return gid;
	}
    static const char *get_group()
    {
		return "unknown";
	}
	void set_time(int which, time_t t)
	{
		time[which] = t;
	}
	time_t get_time(int which) const
	{
		return time[which];
	}
	Vector3 get_text_pos() const override;
	float get_text_size() const override;
};
// --- directories ---

class Dir : public FSNode {
protected:
	std::vector<Dir*> subdirs;
	std::vector<File*> files;
	std::vector<Link> links;

	float min_x, max_x;

	void calc_bounds();
	void place(const Vector3 &pos);

	FSNode *find_intersection(const Ray &ray, float *pt);

public:
	Dir(): min_x(0), max_x(0) {}
    ~Dir() {}

	void add_subdir(Dir *dir)
	{
		subdirs.push_back(dir);
		dir->set_parent(this);

		Link link(this, dir);
		links.push_back(link);
	}

	void add_file(File *file)
	{
		files.push_back(file);
		file->set_parent(this);
	}

	Dir *get_subdirs() const
	{
		return subdirs.empty() ? 0 : (Dir*)&subdirs[0];
	}

	int get_num_subdirs() const
	{
		return (int)subdirs.size();
	}

	File *get_files() const
	{
		return files.empty() ? 0 : (File*)&files[0];
	}

	int get_num_files() const
	{
		return (int)files.size();
	}

	Link *get_links() const
	{
		return links.empty() ? 0 : (Link*)&links[0];
	}

	int get_num_links() const
	{
		return (int)links.size();
	}

	void layout();

    void draw() const override;

    Vector3 get_text_pos() const override;
    float get_text_size() const override;

	virtual bool pick(const Ray &ray);
};

#endif	// FSTREE_H_
