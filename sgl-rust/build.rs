use bindgen;
use std::env;
use std::path::PathBuf;

fn main() {
    println!("cargo:rerun-if-changed=include/sgl.h");

    let bindings = bindgen::Builder::default()
        .header("include/sgl.h")
        .default_enum_style(bindgen::EnumVariation::ModuleConsts)
        .allowlist_type("sglE.*")
        .bitfield_enum("sglE(ClearBit|EnableFlags)")
        .generate()
        .expect("Unable to generate bindigs");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings");
}
