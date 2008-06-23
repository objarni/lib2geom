#include "piecewise.h"
#include "sbasis.h"
#include "bezier-to-sbasis.h"

#include "path-cairo.h"
#include "toy-framework.h"

using namespace Geom;

class PwToy: public Toy {
    unsigned segs, handles_per_seg, handles_per_curve, curves;
    virtual void draw(cairo_t *cr, std::ostringstream *notify, int width, int height, bool save) {
        cairo_set_source_rgba (cr, 0., 0.5, 0, 1);
        cairo_set_line_width (cr, 1);
       
        D2<Piecewise<SBasis> > pws;
        unsigned hdle_idx=0;
        for(unsigned i = 0; i < segs; i++) {
            D2<SBasis> foo = Geom::handles_to_sbasis(handles.begin()+hdle_idx, 3);
            hdle_idx += 4;
            cairo_md_sb(cr, foo);
            for(unsigned d = 0; d < 2; d++) {
                pws[d].cuts.push_back(150*i);
                pws[d].segs.push_back(foo[d]);
            }
        }
        for(unsigned d = 0; d < 2; d++)
            pws[d].cuts.push_back(150*segs);
        
        handles[hdle_idx  ][1]=450;
        handles[hdle_idx+1][1]=450;
        handles[hdle_idx+2][1]=450;
        handles[hdle_idx+3][1]=450;
	
        cairo_set_source_rgba (cr, 0.2, 0.2, 0.2, 1);
        D2<SBasis> foo = Geom::handles_to_sbasis(handles.begin()+hdle_idx, 3);
        SBasis g = foo[0] - Linear(150);
        cairo_md_sb(cr, foo);
	    for(unsigned i=0;i<20;i++){
            double t=i/20.;
            draw_handle(cr, foo(t));
        }
        cairo_stroke(cr);
        foo[1]=foo[0];
        foo[0]=Linear(150,450);
        cairo_md_sb(cr, foo);

        cairo_d2_pw(cr, pws);
        
        cairo_stroke(cr);
        cairo_set_source_rgba (cr, 0.9, 0., 0., 1);
        D2<Piecewise<SBasis> > res = compose(pws, Piecewise<SBasis>(g));
        cairo_d2_pw(cr, res);
        for(unsigned i=0;i<20;i++){
            double t=(res[0].cuts.back()-res[0].cuts.front())*i/20.;
            draw_handle(cr, Point(res[0](t),res[1](t)));
        }
        cairo_stroke(cr);

        Toy::draw(cr, notify, width, height, save);
    }

    bool should_draw_numbers() { return false; }
        
    public:
    PwToy () {
        segs = 2;
        handles_per_seg = 4;
        handles_per_curve = handles_per_seg * segs;
        curves = 1;
        for(unsigned a = 0; a < curves; a++)
            for(unsigned i = 0; i < handles_per_curve; i++)
                handles.push_back(Point(150 + 300*i/(4*segs), uniform() * 150 + 150 - 150 * a));
        for(unsigned i = 0; i < 4; i++)
            handles.push_back(Point(150 + 100*i,100));
    }
};

int main(int argc, char **argv) {
    init(argc, argv, new PwToy());
    return 0;
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4:encoding=utf-8:textwidth=99 :