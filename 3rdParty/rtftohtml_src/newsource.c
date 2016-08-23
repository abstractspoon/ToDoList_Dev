struct Cstyle {
}; /* create an index of these */
struct Pstyle {
}; /* create an index of these */
struct PnStyle {
}; /* pointed to by paragraph */

struct vtext {
	int alloc;
	int used;
	int * ptr;
};

struct TextNode {
	struct vtext vt;
	struct cstyle cs;
	void * next;
};

struct FootnoteNode {
	struct vtext ref;
	struct vtext note;
	struct cstyle cs;
	void * next;
};

struct TOCNode {
	struct vtext ref;
	struct cstyle cs;
	int		noref;
	int		type;
	int 	level;
	void * next;
};

struct IdxNode {
	struct vtext ref;
	struct vtext textval;
	struct vtext BookMark;
	struct cstyle cs;
	int		isbold;
	int		isital;
	int		type;
	int 	level;
	void * next;
};

struct GraphicNode {
	struct cstyle cs;
	long loc;				/* Location of PICT in RTF input */
	int size;				/* Size of PICT in RTF input */
	int Gtype;				/* Type of graphic */
	int PicGoalWid;			
	int PicGoalHt;			
	void * next;
};

