declare class Raw264 {
    constructor(name: string);
    greet(strName: string): string;
    decode(data: ArrayBuffer): object;
    private _addonInstance;
}
export = Raw264;
