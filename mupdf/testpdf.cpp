#include <iostream>
#include <mupdf/fitz.h>

using namespace std;
#define ERROR(message) \
    cout << message << ",error: " << fz_caught_message(ctx) << endl

int render(char *filename, int pagenumber, int zoom, int rotation) {
    // Create a context to hold the exception stack and various caches.
    fz_context *ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (NULL == ctx) {
        cout << "cannot create mupdf context" << endl;
        return EXIT_FAILURE;
    }
    // Register the default file types to handle.
    fz_try(ctx)
    fz_register_document_handlers(ctx);
    fz_catch(ctx) {
        ERROR("cannot register document handlers");
        fz_drop_context(ctx);
        return EXIT_FAILURE;
    }
    // Open the PDF,XPS or CBZ document.
    fz_document *doc;
    fz_try(ctx)
    doc = fz_open_document(ctx, filename);
    fz_catch(ctx) {
        ERROR("cannot open document");
        fz_drop_context(ctx);
        return EXIT_FAILURE;
    }
    // Retrieve the number of pages (not used in this example)
    int pagecount = fz_count_pages(ctx, doc);
    // Load the page we want, Page numbering starts from zero.
    fz_page *page = fz_load_page(ctx, doc, pagenumber - 1);

    // Calculate a transform to use when rendering.This transform contains the
    // scale and rotation.
    // Convert zoom percentage to a scaling factor.Without scaling the
    // resolution is 72 dpi.
    fz_matrix transform;
    transform = fz_scale(zoom / 100.0f, zoom / 100.0f);
    transform = fz_pre_rotate(transform, rotation);

    // Take the page bounds and transform them by the same matrix that we will
    // use
    // to render the page.
    fz_rect bounds;
    bounds = fz_bound_page(ctx, page);
    bounds = fz_transform_rect(bounds, transform);

    // Create a blank pixmap to hold the result of rendering.The
    // pixmap bounds used here the same as the transformed page bounds,
    // so it will contain the entire page. The page coordinate space has
    // the origin at the top left corner and the x axis extends to the
    // right and the y axis extends down

    fz_irect bbox;
    bbox = fz_round_rect(bounds);
    fz_pixmap *pix =
        fz_new_pixmap_with_bbox(ctx, fz_device_rgb(ctx), bbox, NULL, 1);
    fz_clear_pixmap_with_value(ctx, pix, 0xff);
    //
    // A page consists of a series of objects(text, line art, images,gradients).
    // These objects are passed to a device when the interpreter runs the page.
    // There are several devices, used for different purposes:
    //
    // draw device -- renders objects to a target pixmap.
    // text device -- extracts the text in reading order with styling
    // information.
    // This text can be used to provide text search.
    //
    // list device -- records the graphic objects in a list that can be played
    // back through another device.This is useful if you need to run the same
    // page through multiple devices,without the overhead of parsing the page
    // each time.
    //
    // Create a draw device with the pixmap as its target. Run the page with
    // the transform.

    fz_device *dev = fz_new_draw_device(ctx, transform, pix);
    fz_run_page(ctx, page, dev, transform, NULL);
    fz_drop_device(ctx, dev);

    // Save the pixmap to a file.
    fz_output *out = fz_new_output_with_path(ctx, "out.jpg",0);
    fz_write_pixmap_as_png(ctx,out, pix);
    // Clean up.

    fz_drop_pixmap(ctx, pix);
    fz_drop_page(ctx, page);
    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
}

int main(int argc, char *argv[]) {
    cout <<  EXIT_FAILURE << endl;
    //文件路径
    char *filename = "/root/doc.pdf";
    //转换的页面数
    int pagenumber = 2;
    //缩放比例
    int zoom = 100;
    //旋转角度
    int rotation = 60;
    //处理
    render(filename, pagenumber, zoom, rotation);
    return 0;
}
